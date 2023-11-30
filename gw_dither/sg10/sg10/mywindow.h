#pragma once
#include "window5.h"
#include "SimpleFont.h"
#include "stmfont.h"

#include <stdio.h>
#include <math.h>
extern i16 ssdlevels;
extern i16 dithercnt;
namespace sg{

	class Cmywindow;
	class Cmywindow : public CWindow {
	public:
        enum { noimages = 5 +3+1+1+1+3+1+1} ;
		sg::res::CSimpleFont sf;
		sg::res::CstmFont stmf;
		sg::gr::CImg *img,*imgs[noimages];

		float brightness;
		float contrast;
		float gamma;
		int imgidx;

		Cmywindow(Rect r, u32 style,HWND parent_hWnd,u16 *name) : CWindow( r, style, parent_hWnd, name),sf(surf,black),stmf(surf) {
			//end ctor
			imgidx=0;

			brightness = 0;
			contrast = 0;
			gamma = 1;

			surf->do4 = 0;
			surf->do8 = 0;

			
			//img = new sg::gr::CImg("c:/Users/yym/Pictures/testimg2.png.ppm");
			//imgs[0] = new sg::gr::CImg("u:/visc/2022/graphwidget2/pics/g1.png.ppm");
			imgs[0] = new sg::gr::CImg("u:/work/2023/neopixel/gw_dither/pics/colors.ppm");
			//imgs[1] = new sg::gr::CImg("u:/visc/2023/gw_dither/pics/za1.ppm");
			
			
			img = imgs[imgidx];


			ShowWindow(hWnd, SW_SHOWNORMAL);
			UpdateWindow(hWnd);
			//SetTimer(hWnd,9000,16,0);	
		}

#define M_LOG2E 1.44269504088896340736 // log2(e)

        inline long double log2(const long double x){
            return log(x) * M_LOG2E;
        }

		virtual void Paint(HDC dc) {		
			surf->color = white;
			surf->Clear();
			surf->color = red;

			static char text[100];
			sprintf(text,"brightness = %f contrast = %f gamma = %f",brightness, contrast, gamma);

			sf.DrawT(0,0,text);
			
			imgidx = imgidx<0 ? noimages-1 : imgidx>=noimages ? 0 : imgidx;
			img = imgs[imgidx];
			memcpy(img->pData,img->pOrig,320*240*3);
			img->adjustGamma(gamma);
			img->ajdustContrast(contrast);
			img->ajdustBrightness(brightness);
			

			img->draw(surf,20,40);surf->color = red; sf.DrawT(20+(20+320)*0,40 -10,"Original");

            img->drawdither2color(surf, 20 + (20 + 320) * 1, 40); surf->color = red; sf.DrawT(20 + (20 + 320) * 1, 40 - 10, "drawdither2color");
            img->drawdither2colorlev(surf, 20 + (20 + 320) * 2, 40); surf->color = red; sf.DrawT(20 + (20 + 320) * 2, 40 - 10, "drawdither2colorlev");
            img->drawdither8color(surf, 20 + (20 + 320) * 3, 40); surf->color = red; sf.DrawT(20 + (20 + 320) * 3, 40 - 10, "drawdither8color");
            img->draw_steinberg(surf, 20 + (20 + 320) * 0, 40 + 20 + 240); surf->color = red; sf.DrawT(20 + (20 + 320) * 0, 40 + 20 + 240 - 10, "Steinberg");
            img->draw_steinbergcolor(surf, 20 + (20 + 320) * 1, 40 + 20 + 240); surf->color = red; sf.DrawT(20 + (20 + 320) * 1, 40 + 20 + 240 - 10, "draw_steinbergcolor");
            img->drawdither8colorlev(surf, 20 + (20 + 320) * 3, 40 + 20 + 240); surf->color = red; sf.DrawT(20 + (20 + 320) * 3, 40 + 20 + 240 - 10, "drawdither8colorlev");
            img->drawdither16(surf, 20 + (20 + 320) * 2, 40 + 20 + 240); surf->color = red; sf.DrawT(20 + (20 + 320) * 2, 40 + 20 + 240 - 10, "drawdither16");

            char buff[256];
            sprintf(buff, "burkes levels=%d colors = %d,bits=%f", ssdlevels, ssdlevels*ssdlevels*ssdlevels, log2(ssdlevels*ssdlevels*ssdlevels));

			img->draw_dither_core(surf,20+(20+320)*0,40+(20+240)*2,0); surf->color = red; sf.DrawT(20+(20+320)*0,40+(20+240)*2-10,"draw_dither_core");
			img->draw_dither_core(surf,20+(20+320)*1,40+(20+240)*2,1); surf->color = red; sf.DrawT(20+(20+320)*1,40+(20+240)*2-10,"draw_dither_core");
            img->draw_burkescolor(surf, 20 + (20 + 320) * 2, 40 + (20 + 240) * 2); surf->color = red; sf.DrawT(20 + (20 + 320) * 2, 40 + (20 + 240) * 2 - 10, buff);

            img->draw_burkescolor2(surf, 20 + (20 + 320) * 3, 40 + (20 + 240) * 2); surf->color = red; sf.DrawT(20 + (20 + 320) * 3, 40 + (20 + 240) * 2 - 10, "draw_burkescolor2");


			surf->Paint(dc);		
		}
        u8 getbits(u32 offs){
            u8 *pdata = &scopy[offs];
            u8 res = 0;
            for (int i = 0; i < 8; i++){
                res |= *pdata++ ? (0x80 >> i) : 0;
            }
            return res;
        }
        void save_scopy(){
            FILE *f;
            f = fopen("myimg.c","wt");
            if (!f) return;
            fprintf(f, "#include <stdint.h>\n");
            fprintf(f,"uint8_t myimg[320 / 8 * 240] = { \n");
            u32 offs = 0;
            for (int y = 0; y < 240; y++){
                for (int x = 0; x < 320 / 8; x++){
                    fprintf(f, "0x%x, ", getbits(offs));
                        offs += 8;
                }
                fprintf(f, "\n");
            }
            fprintf(f, "};");
            fclose(f);
        }
        
		virtual LRESULT CALLBACK WndProc(UINT Msg,WPARAM wParam, LPARAM lParam) {
			static bool playing = true;
			LRESULT res = 0;
			switch(Msg)
			{
			/*case WM_TIMER:
				InvalidateRect(hWnd,0,0);
				break;*/

			case WM_CHAR:
				switch(wParam)
				{
				case '9': contrast +=2;InvalidateRect(hWnd,0,0);break;
				case '7': contrast -=2;InvalidateRect(hWnd,0,0);break;
				case '8': contrast =0;InvalidateRect(hWnd,0,0);break;

				case '6': brightness +=2;InvalidateRect(hWnd,0,0);break;
				case '4': brightness -=2;InvalidateRect(hWnd,0,0);break;
				case '5': brightness =0;InvalidateRect(hWnd,0,0);break;

				case '3': gamma *=1.025;InvalidateRect(hWnd,0,0);break;
				case '1': gamma /=1.025;InvalidateRect(hWnd,0,0);break;
				case '2': gamma =1;InvalidateRect(hWnd,0,0);break;

				case '/': imgidx++;InvalidateRect(hWnd,0,0);break;
				case '*': imgidx--;InvalidateRect(hWnd,0,0);break;

                case '+':ssdlevels++; InvalidateRect(hWnd, 0, 0); break;
                case '-': ssdlevels--; if (ssdlevels < 2)ssdlevels = 2; InvalidateRect(hWnd, 0, 0); break;

                case 'q': PostQuitMessage(0); break;
				case'w':  SetTimer(hWnd,36100,1,0); break;
				case'e':  KillTimer(hWnd,36100); break;
					 case 'r':dithercnt++; InvalidateRect(hWnd, 0, 0); break;
  				case 's':
                    save_scopy();
                    break;
				}
				break;
			case WM_TIMER:
				dithercnt++; 
				InvalidateRect(hWnd, 0, 0); break;
			}
			return 0;
		}

	};
};// sg


