/*
 *  SVG.c
 *  XingXiangTu
 *
 *  Created by Joe Ho on 3/6/2552 日23.
 *
 */
#include <stdio.h>
#include <math.h>
//#include "SVG.h"

//#define kFALSE 0
//#define kTRUE 1


/*
void chuNhat( FILE *tep, float gocX, float gocY, float beRong, float beCao, unsigned char toDay, unsigned int mauToDay, float doDucToDay,
          unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet );
 
void vongTron( FILE *tep, float tamX, float tamY, float banKinh, unsigned char toDay, unsigned int mauToDay, float doDucToDay,
            unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet );
 
void elip( FILE *tep, float tamX, float tamY, float banKinhX, float banKinhY, unsigned char toDay, unsigned int mauToDay, float doDucToDay,
             unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet );
 
void duong( FILE *tep, float diemX0, float diemY0, float diemX1, float diemY1, float beRongNet, unsigned int mauNet, float doDucNet );
 
void duongDaDiem( FILE *tep, float *mangDiem, unsigned int soLuongDiem, unsigned char toDay, unsigned int mauToDay, float doDucToDay,
                    unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet );
 
 void polygonPoints( FILE *tep, float *mangDiem, unsigned int soLuongDiem, unsigned char toDay, unsigned int mauToDay, float doDucToDay,
 unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet );
 
 void duongCong( FILE *tep, float diemBatDauX, float diemBatDauY, float *mangDiem, unsigned int soLuongKhuc,
 unsigned char toDay, unsigned int mauToDay, float doDucToDay, unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet )
 
 void vanBan_ngang( FILE *tep, char *xau, float viTriX, float viTriY, char *giaDinhPhong, float coKichPhong, char *beDayPhong,
 unsigned char toDay, unsigned int mauToDay, float doDucToDay, unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet )
 
 void vanBan_doc( FILE *tep, char *xau, float viTriX, float viTriY, char *giaDinhPhong, float coKichPhong, char *beDayPhong,
 unsigned char toDay, unsigned int mauToDay, float doDucToDay, unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet )
 
 void hinhTronChuyenSac_khongNet( FILE *tep, float viTriX, float viTriY, float banKinh, unsigned int *mangMau, float *mangViTriBuoc,
 float *mangBuoc, unsigned short soLuongBuoc, unsigned int maSoChuyenSac )
 */

// Vẽ hình chữ nhật tại góc trái dưới tại (gocX; gocY)
void chuNhat( FILE *tep, float gocX, float gocY, float beRong, float beCao, unsigned char toDay, unsigned int mauToDay, float doDucToDay,
         unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet ) {
   
	fprintf( tep, "<rect x=\"%5.1f\" y=\"%5.1f\" width=\"%5.1f\" height=\"%5.1f\" ", gocX, gocY, beRong, beCao );
	if( toDay )
	   fprintf( tep, "fill=\"#%06X\" fill-opacity=\"%4.2f\" ", mauToDay, doDucToDay );
	else
	   fprintf( tep, "fill=\"none\" " );
	if( net ) {
	   fprintf( tep, "stroke=\"#%06X\" stroke-opacity=\"%4.2f\" ", mauNet, doDucNet );
	   fprintf( tep, "stroke-width=\"%5.1f\" ", beRongNet );
	}
	else {
	   fprintf( tep, "stroke=\"none\" " );
	}

   fprintf( tep, "/>\n\n" );
}


// Vẽ hình tròn tại điểm (tamX; tamY)
void hinhTron( FILE *tep, float tamX, float tamY, float banKinh, unsigned char toDay, unsigned int mauToDay, float doDucToDay,
         unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet ) {
   
	fprintf( tep, "<circle cx=\"%5.1f\" cy=\"%5.1f\" r=\"%5.1f\" ", tamX, tamY, banKinh );
	if( toDay )
	   fprintf( tep, "fill=\"#%06X\" fill-opacity=\"%4.2f\" ", mauToDay, doDucToDay );
	else
	   fprintf( tep, "fill=\"none\" " );
	if( net ) {
	   fprintf( tep, "stroke=\"#%06X\" stroke-opacity=\"%4.2f\" ", mauNet, doDucNet );
	   fprintf( tep, "stroke-width=\"%5.1f\" ", beRongNet );
	}
	else {
	   fprintf( tep, "stroke=\"none\" " );
	}

   fprintf( tep, "/>\n\n" );
}


// Vẽ hình elips
void elip( FILE *tep, float tamX, float tamY, float banKinhX, float banKinhY, unsigned char toDay, unsigned int mauToDay, float doDucToDay,
         unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet ) {

	fprintf( tep, "<ellipse cx=\"%5.1f\" cy=\"%5.1f\" rx=\"%5.1f\" ry=\"%5.1f\" ", tamX, tamY, banKinhX, banKinhY );
	if( toDay )
	   fprintf( tep, "fill=\"#%06X\" fill-opacity=\"%4.2f\" ", mauToDay, doDucToDay );
	else
	   fprintf( tep, "fill=\"none\" " );
	if( net ) {
	   fprintf( tep, "stroke=\"#%06X\" stroke-opacity=\"%4.2f\" ", mauNet, doDucNet );
	   fprintf( tep, "stroke-width=\"%5.1f\" ", beRongNet );
	}
	else {
	   fprintf( tep, "stroke=\"none\" " );
	}

   fprintf( tep, "/>\n\n" );
}


// Vẽ một nét giữa hai điểm
void duong( FILE *tep, float diemX0, float diemY0, float diemX1, float diemY1, float beRongNet, unsigned int mauNet, float doDucNet ) {
   fprintf( tep, "<line x1=\"%5.1f\" y1=\"%5.1f\" x2=\"%5.1f\" y2=\"%5.1f\" stroke=\"#%06X\" stroke-opacity=\"%4.2f\" stroke-width=\"%5.1f\" />\n",
	               diemX0, diemY0, diemX1, diemY1, mauNet, doDucNet, beRongNet );
	
}


// Để vẽ một dãy nét
// mảng phải có điểm sắp xếp với nhau: x0; y0; x1; y1; ...
void duongDaDiem( FILE *tep, float *mangDiem, unsigned int soLuongDiem, unsigned char toDay, unsigned int mauToDay, float doDucToDay,
                    unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet ) {
   int chiSo = 0; // points are interlaced

   fprintf( tep, "<polyline points=\"" );
   while( chiSo < soLuongDiem ) {
	   fprintf( tep, "%5.1f,%5.1f  ", mangDiem[chiSo << 1], mangDiem[(chiSo << 1) + 1] );
      if( chiSo % 10 == 0 )
		   fprintf( tep, "\n" );
      chiSo++;
	}

	fprintf( tep, "\"\n" );
	if( toDay )
	   fprintf( tep, "fill=\"#%06X\" fill-opacity=\"%4.2f\" ", mauToDay, doDucToDay );
	else
	   fprintf( tep, "fill=\"none\" " );
	if( net ) {
	   fprintf( tep, "stroke=\"#%06X\" stroke-opacity=\"%4.2f\" ", mauNet, doDucNet );
	   fprintf( tep, "stroke-width=\"%5.1f\" ", beRongNet );
	}
	else {
	   fprintf( tep, "stroke=\"none\" " );
	}

   fprintf( tep, "/>\n\n" );
}


// Để vẽ hình đa giác
// mảng phải có điểm sắp xếp với nhau: x0; y0; x1; y1; ...
void daGiac( FILE *tep, float *mangDiem, unsigned int soLuongDiem, unsigned char toDay, unsigned int mauToDay, float doDucToDay,
                    unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet ) {
   int chiSoDiem = 0;

   fprintf( tep, "<polygon points=\"" );
   while( chiSoDiem < soLuongDiem ) {
	   fprintf( tep, "%5.1f,%5.1f  ", mangDiem[chiSoDiem << 1], mangDiem[(chiSoDiem << 1) + 1] );
      
      // ---- cho dễ nhìn trong tệp SVG
      if( chiSoDiem % 10 == 0 )
		   fprintf( tep, "\n" );
      
      chiSoDiem++;
	}

	fprintf( tep, "\"\n" );
	if( toDay )
	   fprintf( tep, "fill=\"#%06X\" fill-opacity=\"%4.2f\" ", mauToDay, doDucToDay );
	else
	   fprintf( tep, "fill=\"none\" " );
	if( net ) {
	   fprintf( tep, "stroke=\"#%06X\" stroke-opacity=\"%4.2f\" ", mauNet, doDucNet );
	   fprintf( tep, "stroke-width=\"%5.1f\" ", beRongNet );
	}
	else {
	   fprintf( tep, "stroke=\"none\" " );
	}

   fprintf( tep, "/>\n\n" );
}

void daGiacChuyenSac( FILE *tep, float *mangDiem, unsigned int soLuongDiem, unsigned int maSoChuyenSac ) {
   int chiSoDiem = 0;
   
   fprintf( tep, "<polygon points=\"" );
   while( chiSoDiem < soLuongDiem ) {
      fprintf( tep, "%5.1f,%5.1f  ", mangDiem[chiSoDiem << 1], mangDiem[(chiSoDiem << 1) + 1] );
      
      // ---- cho dễ nhìn trong tệp SVG
      if( chiSoDiem % 10 == 0 )
         fprintf( tep, "\n" );
      
      chiSoDiem++;
   }
   
   fprintf( tep, "\"\n" );

   fprintf( tep, "fill=\"url(#lg%d)\" ", maSoChuyenSac );
   fprintf( tep, "stroke=\"none\" " );

   fprintf( tep, "/>\n\n" );
}

// mảng có dữ liệu dạng: điểmĐiềuKhiển_X1, điểmĐiềuKhiển_Y1, điểmĐiềuKhiển_X2, điểmĐiềuKhiển_Y2, ... điểmKếtThúc_X, điểmKếtThúc_Y
void duongCong( FILE *tep, float diemBatDauX, float diemBatDauY, float *mangDiem, unsigned int soLuongKhuc,
                    unsigned char toDay, unsigned int mauToDay, float doDucToDay, unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet ) {
   int chiSoKhuc = 0;

   fprintf( tep, "<path d=\" M%5.1f,%5.1f", diemBatDauX, diemBatDauY );
   while( chiSoKhuc < soLuongKhuc ) {
	   fprintf( tep, "\nC %5.1f,%5.1f %5.1f,%5.1f %5.1f,%5.1f",
		   mangDiem[chiSoKhuc*6], mangDiem[chiSoKhuc*6 + 1],
		   mangDiem[chiSoKhuc*6 + 2], mangDiem[chiSoKhuc*6 + 3],
		   mangDiem[chiSoKhuc*6 + 4], mangDiem[chiSoKhuc*6 + 5] );
      chiSoKhuc++;
   }
	fprintf( tep, "\"\n" );
	if( toDay )
	   fprintf( tep, "fill=\"#%06X\" fill-opacity=\"%4.2f\" ", mauToDay, doDucToDay );
	else
	   fprintf( tep, "fill=\"none\" " );
	if( net ) {
	   fprintf( tep, "stroke=\"#%06X\" stroke-opacity=\"%4.2f\" ", mauNet, doDucNet );
	   fprintf( tep, "stroke-width=\"%5.1f\" ", beRongNet );
	}
	else {
	   fprintf( tep, "stroke=\"none\" " );
	}

   fprintf( tep, "/>\n\n" );
}


void vanBan_ngang( FILE *tep, char *xau, float viTriX, float viTriY, char *giaDinhPhong, float coKichPhong, char *beDayPhong,
                       unsigned char toDay, unsigned int mauToDay, float doDucToDay, unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet ) {

   fprintf( tep, "<text x=\"%5.1f\" y=\"%5.1f\" font-family=\"%s\" font-size=\"%5.1f\" font-weight=\"%s\" writing-mode=\"lr\" ", viTriX, viTriY, giaDinhPhong, coKichPhong, beDayPhong );
	if( toDay )
	   fprintf( tep, "fill=\"#%06X\" fill-opacity=\"%4.2f\" ", mauToDay, doDucToDay );
	else
	   fprintf( tep, "fill=\"none\" " );
	if( net ) {
	   fprintf( tep, "stroke=\"#%06X\" stroke-opacity=\"%4.2f\" ", mauNet, doDucNet );
	   fprintf( tep, "stroke-width=\"%5.1f\" ", beRongNet );
	}
	else {
	   fprintf( tep, "stroke=\"none\" " );
	}
   fprintf( tep, ">\n" );
	fprintf( tep, " %s </text>\n\n", xau );
}


void vanBan_doc( FILE *tep, char *xau, float viTriX, float viTriY, char *giaDinhPhong, float coKichPhong, char *beDayPhong,
				         unsigned char toDay, unsigned int mauToDay, float doDucToDay, unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet ) {

   fprintf( tep, "<text x=\"%5.1f\" y=\"%5.1f\" font-family=\"%s\" font-size=\"%5.1f\" font-weight=\"%s\" writing-mode=\"tb\" ", viTriX, viTriY, giaDinhPhong, coKichPhong, beDayPhong );
	if( toDay )
	   fprintf( tep, "fill=\"#%06X\" fill-opacity=\"%4.2f\" ", mauToDay, doDucToDay );
	else
	   fprintf( tep, "fill=\"none\" " );
	if( net ) {
	   fprintf( tep, "stroke=\"#%06X\" stroke-opacity=\"%4.2f\" ", mauNet, doDucNet );
	   fprintf( tep, "stroke-width=\"%5.1f\" ", beRongNet );
	}
	else {
	   fprintf( tep, "stroke=\"none\" " );
	}
   fprintf( tep, ">\n" );
	fprintf( tep, " %s </text>\n\n", xau );
}


void hinhTronChuyenSac_khongNet( FILE *tep, float viTriX, float viTriY, float banKinh, unsigned int *mangMau, float *mangViTriBuoc,
                    float *mangBuoc, unsigned short soLuongBuoc, unsigned int maSoChuyenSac ) {
   unsigned short stopIndex;

   fprintf( tep, "<defs>\n" );
	fprintf( tep, " <radialGradient id=\"rg%d\" cx=\"0.5\" cy=\"0.5\">\n", maSoChuyenSac );
   
   unsigned short chiSoBuoc = 0;
   
   // các bước cho chuyển sắc
   while( chiSoBuoc < soLuongBuoc ) {
      fprintf( tep, "  <stop offset=\"%5.2f\" stop-color=\"#%06X\" stop-opacity=\"%5.2f\"/>\n", mangViTriBuoc[chiSoBuoc], mangMau[chiSoBuoc],
		                mangBuoc[chiSoBuoc] );
      chiSoBuoc++;
   }
   
   fprintf( tep, " </radialGradient>\n" );

   fprintf( tep, "</defs>\n\n" );
 
	fprintf( tep, "<circle cx=\"%5.1f\" cy=\"%5.1f\" r=\"%5.1f\" ", viTriX, viTriY, banKinh );
	
	fprintf( tep, "fill=\"url(#rg%d)\" ", maSoChuyenSac );
	fprintf( tep, "stroke=\"none\" " );

   fprintf( tep, "/>\n\n" );
}

#pragma mark ==== Chuyển Sắc
void datChuyeSacTuyenTinh( FILE *tep, unsigned int *mangMau, float *mangViTriBuoc,
                        unsigned short soLuongBuoc, unsigned int maSoChuyenSac, float viTriDauX, float viTriDauY, float viTriCuoiX, float viTriCuoiY ) {

   fprintf( tep, " <linearGradient id=\"lg%d\" gradientUnits=\"userSpaceOnUse\" x1=\"%5.1f\" y1=\"%5.1f\" x2=\"%5.1f\" y2=\"%5.1f\">\n", maSoChuyenSac, viTriDauX,viTriDauY, viTriCuoiX, viTriCuoiY );

   // các bước cho chuyển sắc
   unsigned short chiSoBuoc = 0;
   while( chiSoBuoc < soLuongBuoc ) {
      fprintf( tep, "  <stop offset=\"%5.2f\" stop-color=\"#%06X\" stop-opacity=\"1.0\"/>\n", mangViTriBuoc[chiSoBuoc], mangMau[chiSoBuoc] );
      chiSoBuoc++;
   }

   fprintf( tep, " </linearGradient>\n" );
}


void taoTuGiac() {

   //           2
   // 0 +------+-----> +x
   //   |      |
   //   |      |
   // 4 +      + 5 <--- chỉ cho đặt màu chuyển sắc
   //   |      |
   //   |      |
   // 1 +------+ 3
   //   |
   //   v +y
   
   // ===================== DỮ LIỆU
   // ---- vùng tư giác
   float diem0_x = 1421.0f; //807.0f;//906.0f;//226.0f;
   float diem0_y = 1083.0f;//900.0f;//780.0f;//754.0f;
   
   float diem1_x = 980.0f;//888.0f;//906.0f;//842.0f;
   float diem1_y = 1322.0f;//1301.0f;//1280.0f;//1294.0f;
   
   float diem2_x = 1622.0f;//906.0f;//1421.0f;//807.0f;
   float diem2_y = 1139.0f;//780.0f;//1083.0f;//900.0f;
   
   float diem3_x = 996.0f;//906.0f;//980.0f;//888.0f;
   float diem3_y = 1326.0f;//1280.0f;//1323.0f;//1301.0f;
   
   // ----- màu
   unsigned char diem0_do = 248;//238; //248;//248;
   unsigned char diem0_luc = 255; //248; //248;//255;
   unsigned char diem0_xanh = 248;//238; //248;//248;
   
   unsigned char diem1_do = 0;
   unsigned char diem1_luc = 0;
   unsigned char diem1_xanh = 0;
   
   //-----
   unsigned char diem4_do = 88;//0;//127;//0;
   unsigned char diem4_luc = 84;//100;//127;//127;
   unsigned char diem4_xanh = 150;//144;//127;//0;
   
   unsigned char diem5_do = 88;//100;//0;//0;
   unsigned char diem5_luc = 0;//100; //61;//127;
   unsigned char diem5_xanh = 127;//100; //181;//157;
   
   // ------
   unsigned char diem2_do = 238;//238;//251;//248;
   unsigned char diem2_luc = 238;//238;//248; //255;
   unsigned char diem2_xanh = 255;//238;//255;//250;
   
   unsigned char diem3_do = 0;
   unsigned char diem3_luc = 0;
   unsigned char diem3_xanh = 0;
   
   
   // =========================== TÍNH BƯỚC
   
   char soLuongBuoc = 10;
   
   // ---- tạo miến dán đầy tư giác
   float buocTren_x = (diem2_x - diem0_x)/(float)soLuongBuoc;
   float buocTren_y = (diem2_y - diem0_y)/(float)soLuongBuoc;
   
   float buocDuoi_x = (diem3_x - diem1_x)/(float)soLuongBuoc;
   float buocDuoi_y = (diem3_y - diem1_y)/(float)soLuongBuoc;
   
   // ---- màu
   float buocTren_do = (float)(diem2_do - diem0_do)/(float)soLuongBuoc;
   float buocTren_luc = (float)(diem2_luc - diem0_luc)/(float)soLuongBuoc;
   float buocTren_xanh = (float)(diem2_xanh - diem0_xanh)/(float)soLuongBuoc;
   
   float buocGiua_do = (float)(diem5_do - diem4_do)/(float)soLuongBuoc;
   float buocGiua_luc = (float)(diem5_luc - diem4_luc)/(float)soLuongBuoc;
   float buocGiua_xanh = (float)(diem5_xanh - diem4_xanh)/(float)soLuongBuoc;
   
   
   float buocDuoi_do = (float)(diem3_do - diem1_do)/(float)soLuongBuoc;
   float buocDuoi_luc = (float)(diem3_luc - diem1_luc)/(float)soLuongBuoc;
   float buocDuoi_xanh = (float)(diem3_xanh - diem1_xanh)/(float)soLuongBuoc;
   
   // =========================== TÍNH VÀ GHI TỆP
   // ---- mở tập tin SVG
   FILE *tep = fopen( "tuGiac.svg", "w" );
   
   if( tep != NULL ) {
      
      // ---- đầu tập tin SVG
      unsigned int beRong = 1400;
      unsigned int beCao = 1000;
      fprintf( tep, "<svg version=\"1.1\" width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n\n", beRong, beCao );
      
      // ---- mảng điểm tư giác
      float mangTuGiac[8];
      
      mangTuGiac[0] = diem0_x;
      mangTuGiac[1] = diem0_y;
      
      mangTuGiac[2] = diem1_x;
      mangTuGiac[3] = diem1_y;
      
      float mauDoTren = (float)diem0_do;
      float mauLucTren = (float)diem0_luc;
      float mauXanhTren = (float)diem0_xanh;
      
      float mauDoGiua = (float)diem4_do;
      float mauLucGiua = (float)diem4_luc;
      float mauXanhGiua = (float)diem4_xanh;
      
      float mauDoDuoi = (float)diem1_do;
      float mauLucDuoi = (float)diem1_luc;
      float mauXanhDuoi = (float)diem1_xanh;
      
      unsigned int maSoChuyenSac = 0x0234;
      
      unsigned short buoc = 0;
      while( buoc < soLuongBuoc ) {
         
         // ---- vị trí
         mangTuGiac[4] = mangTuGiac[2] + buocDuoi_x*1.01;
         mangTuGiac[5] = mangTuGiac[3] + buocDuoi_y*1.01;
         
         mangTuGiac[6] = mangTuGiac[0] + buocTren_x*1.01;
         mangTuGiac[7] = mangTuGiac[1] + buocTren_y*1.01;
         
         // ---- màu
         unsigned char mauDo_0 = (unsigned char)mauDoTren;
         unsigned char mauLuc_0 = (unsigned char)mauLucTren;
         unsigned char mauXanh_0 = (unsigned char)mauXanhTren;
         
         unsigned char mauDo_1 = (unsigned char)mauDoGiua;
         unsigned char mauLuc_1 = (unsigned char)mauLucGiua;
         unsigned char mauXanh_1 = (unsigned char)mauXanhGiua;
         
         unsigned char mauDo_2 = (unsigned char)mauDoDuoi;
         unsigned char mauLuc_2 = (unsigned char)mauLucDuoi;
         unsigned char mauXanh_2 = (unsigned char)mauXanhDuoi;
         
         unsigned int mangMau[3];
         mangMau[0] = mauDo_0 << 16 | mauLuc_0 << 8 | mauXanh_0;
         mangMau[1] = mauDo_1 << 16 | mauLuc_1 << 8 | mauXanh_1;
         mangMau[2] = mauDo_2 << 16 | mauLuc_2 << 8 | mauXanh_2;
         
         float mangBuocChuyenSac[3] = {0.0f, 0.5f, 1.0f};
         maSoChuyenSac++;
         
         float diemChuyenSac0_x = mangTuGiac[0] + buocTren_x*0.5f;
         float diemChuyenSac0_y = mangTuGiac[1] + buocTren_y*0.5f;
         
         float diemChuyenSac1_x = mangTuGiac[2] + buocDuoi_x*0.5f;
         float diemChuyenSac1_y = mangTuGiac[3] + buocDuoi_y*0.5f;
         
         // ---- đặt chuyển sắc
         fprintf( tep, "<defs>\n" );
         datChuyeSacTuyenTinh( tep, mangMau, mangBuocChuyenSac, 3, maSoChuyenSac,
                              diemChuyenSac0_x, diemChuyenSac0_y, diemChuyenSac1_x, diemChuyenSac1_y );
         fprintf( tep, "</defs>\n\n" );
         
         // ---- vẽ tư giác
         //   daGiac( tep, mangTuGiac, 4, 1, mangMau[1], 1.0f, 0, 0.0f, 0x00, 1.0f );
         daGiacChuyenSac( tep, mangTuGiac, 4, maSoChuyenSac );
         
         // ---- nâng cấp điểm
         mangTuGiac[0] += buocTren_x;
         mangTuGiac[1] += buocTren_y;
         
         mangTuGiac[2] += buocDuoi_x;
         mangTuGiac[3] += buocDuoi_y;
         
         // ---- nâng cấp màu
         mauDoTren += buocTren_do;
         mauLucTren += buocTren_luc;
         mauXanhTren += buocTren_xanh;
         
         mauDoGiua += buocGiua_do;
         mauLucGiua += buocGiua_luc;
         mauXanhGiua += buocGiua_xanh;
         
         mauDoDuoi += buocDuoi_do;
         mauLucDuoi += buocDuoi_luc;
         mauXanhDuoi += buocDuoi_xanh;
         
         buoc++;
         printf( "buoc %d  (%5.1f;  %5.1f) (%5.1f;  %5.1f)\n", buoc,
                diemChuyenSac0_x, diemChuyenSac0_y, diemChuyenSac1_x, diemChuyenSac1_y );
      }
      
      // ----
      fprintf( tep, "</svg>\n" );
      // ---- đóng tệp
      fclose( tep );
   }
}

#define kPI 3.1415927f

void tinhMauChoGoc( float goc, float *mauDo, float *mauLuc, float *mauXanh ) {
   
   if( goc < kPI/3.0f ) {   // màu đỏ
      float donViHoa = goc*3.0f/kPI;
      *mauDo = 1.0f;
      *mauLuc = donViHoa;
      *mauXanh = 0.0f;
   }
   else if( goc < 2.0f*kPI/3.0f ) {    // màu vàng
      float donViHoa = (goc - kPI/3.0f)*3.0f/kPI;
      *mauDo = 1.0f - donViHoa;
      *mauLuc = 1.0f;
      *mauXanh = 0.0f;
   }
   else if( goc < kPI ) {     // màu lúc
      float donViHoa = (goc - 2.0f*kPI/3.0f)*3.0f/kPI;
      *mauDo = 0.0f;
      *mauLuc = 1.0f;
      *mauXanh = donViHoa;
   }
   else if( goc < 4.0f*kPI/3.0f ) {    // màu xanh da trời
      float donViHoa = (goc - kPI)*3.0f/kPI;
      *mauDo = 0.0f;
      *mauLuc = 1.0f - donViHoa;
      *mauXanh = 1.0f;
   }
   else if( goc < 5.0f*kPI/3.0f ) {   // màu xanh
      float donViHoa = (goc - 4.0f*kPI/3.0f)*3.0f/kPI;
      *mauDo = donViHoa;
      *mauLuc = 0.0f;
      *mauXanh = 1.0f;
   }
   else if( goc < 2.0f*kPI ) {   // màu xanh
      float donViHoa = (goc - 5.0f*kPI/3.0f)*3.0f/kPI;
      *mauDo = 1.0f;
      *mauLuc = 0.0f;
      *mauXanh = 1.0f - donViHoa;
   }
   else {
      *mauDo = 0.0f;
      *mauLuc = 0.0f;
      *mauXanh = 0.0f;
   }
   
   
   // ---- cho sáng
   *mauDo *= 0.5f;
   *mauLuc *= 0.5f;
   *mauXanh *= 0.5f;
   *mauDo += 0.5f;
   *mauLuc += 0.5f;
   *mauXanh += 0.5f;
   
   // ---- nhân 255.0f
   *mauDo *= 255.0f;
   *mauLuc *= 255.0f;
   *mauXanh *= 255.0f;
}

void taoHinhTron() {
   
   float banKinh = 214.0f;
   
   float gocX = 690;//1440.0f;
   float gocY = 380.0f;//410.0f;

   // ---- tính trung tâm
   float trungTamX = gocX + banKinh;
   float trungTamY = gocY + banKinh;

   // ----
   unsigned short soLuongBuoc = 180;
   
   float gocMien = kPI*2.0f/(float)soLuongBuoc;
   
   // ---- mở tập tin SVG
   FILE *tep = fopen( "hinhTronGiac.svg", "w" );
   
   if( tep != NULL ) {
      
      // ---- đầu tập tin SVG
      unsigned int beRong = 1400;
      unsigned int beCao = 1000;
      fprintf( tep, "<svg version=\"1.1\" width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n\n", beRong, beCao );

      float mangTamGiac[6];
      
      // ---- vị trí
      mangTamGiac[0] = trungTamX + banKinh;
      mangTamGiac[1] = trungTamY;
      
      mangTamGiac[4] = trungTamX;
      mangTamGiac[5] = trungTamY;
      
      unsigned short buoc = 0;
      float gocHienTai = gocMien;
      
      // ---- màu trung tâm
      unsigned int mauTrungTam = 0xcfcfcf;//0x7f7f7f;
      
      unsigned int maSoChuyenSac =  0x456;
      
      while( buoc < soLuongBuoc ) {
         
         mangTamGiac[2] = trungTamX + banKinh*cosf( gocHienTai + 0.01f );
         mangTamGiac[3] = trungTamY + banKinh*sinf( gocHienTai + 0.01f );
         
         // ---- tính màu ranh giới nhờ góc
         float mauDoRanh = 0.0f;
         float mauLucRanh = 0.0f;
         float mauXanhRanh = 0.0f;
         tinhMauChoGoc( gocHienTai, &mauDoRanh, &mauLucRanh, &mauXanhRanh );
         
         // ---- màu
         unsigned char mauDo_0 = (unsigned char)mauDoRanh;
         unsigned char mauLuc_0 = (unsigned char)mauLucRanh;
         unsigned char mauXanh_0 = (unsigned char)mauXanhRanh;
         
         unsigned int mauRanh = mauDo_0 << 16 | mauLuc_0 << 8 | mauXanh_0;
         
         unsigned int mangMau[2];
         mangMau[0] = mauTrungTam;
         mangMau[1] = mauRanh;

         
         float mangBuocChuyenSac[3] = {0.0f, 1.0f};
         maSoChuyenSac++;
         
         float diemChuyenSac0_x = trungTamX;
         float diemChuyenSac0_y = trungTamY;
         
         float diemRanh_x = trungTamX + banKinh*cosf( gocHienTai + 0.5f*gocMien);
         float diemRanh_y = trungTamY + banKinh*sinf( gocHienTai + 0.5f*gocMien);
         
         // ---- đặt chuyển sắc
         fprintf( tep, "<defs>\n" );
         datChuyeSacTuyenTinh( tep, mangMau, mangBuocChuyenSac, 2, maSoChuyenSac,
                              trungTamX,trungTamY, diemRanh_x, diemRanh_y );
         fprintf( tep, "</defs>\n\n" );

         // ---- vẽ tam giác
         //daGiac( tep, mangTamGiac, 3, 1, mauRanh, 1.0f, 0, 0.0f, 0x00, 1.0f );
         daGiacChuyenSac( tep, mangTamGiac, 3, maSoChuyenSac );
         
         // ---- nâng cấp
         mangTamGiac[0] = trungTamX + banKinh*cosf( gocHienTai );
         mangTamGiac[1] = trungTamY + banKinh*sinf( gocHienTai );
         gocHienTai += gocMien;
         
         maSoChuyenSac++;
         buoc++;
      }
      
      // ----
      fprintf( tep, "</svg>\n" );
      // ---- đóng tệp
      fclose( tep );
   }
}

void veNetGiuaHaiHinhElip() {
   
   float elip0_gocX = 1319.8f;
   float elip0_gocY = 590.6f;
   float elip0_banKinhX = 115.2f;
   float elip0_banKinhY = 68.4f;
   
   float elip1_gocX = 1422.2f;
   float elip1_gocY = 781.4f;
   float elip1_banKinhX = 12.8f;
   float elip1_banKinhY = 7.6f;
   
   // ---- tính trung tâm
   float elip0_trungTamX = elip0_gocX + elip0_banKinhX;
   float elip0_trungTamY = elip0_gocY + elip0_banKinhY;
   
   float elip1_trungTamX = elip1_gocX + elip1_banKinhX;
   float elip1_trungTamY = elip1_gocY + elip1_banKinhY;
   printf( "trung tâm 0 (%5.1f; %5.1f)  trung tâm 1 (%5.1f; %5.1f)\n", elip0_trungTamX, elip0_trungTamY, elip1_trungTamX, elip1_trungTamY );
   
   // ---- mở tập tin SVG
   FILE *tep = fopen( "netGiuaHaiHinhElip.svg", "w" );
   
   if( tep != NULL ) {
      
      // ---- đầu tập tin SVG
      unsigned int beRong = 1400;
      unsigned int beCao = 1000;
      fprintf( tep, "<svg version=\"1.1\" width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n\n", beRong, beCao );
      
      // ---- số lượng bước
      unsigned short soLuongBuoc = 32;
      
      float gocBuoc = kPI*2.0f/(float)soLuongBuoc;
      
      unsigned short buoc = 0;
      float gocHienTai = 2.0*kPI/64.0f;
      
      while( buoc < soLuongBuoc ) {
         
         // ---- vị trí
         float diemRanh0_x = elip0_trungTamX + elip0_banKinhX*cosf( gocHienTai );
         float diemRanh0_y = elip0_trungTamY + elip0_banKinhY*sinf( gocHienTai );
         
         float diemRanh1_x = elip1_trungTamX + elip1_banKinhX*cosf( gocHienTai );
         float diemRanh1_y = elip1_trungTamY + elip1_banKinhY*sinf( gocHienTai );
         
         // ---- màu
         float giaTri = 128.0f*(0.5f + 0.5f*sinf( gocHienTai ));
         unsigned int hieuUngSang = floorf( giaTri );
         
         // ---- vẽ nét
         duong( tep, diemRanh0_x, diemRanh0_y, diemRanh1_x, diemRanh1_y, 1.5f, 0xff5a00 + (hieuUngSang << 8), 1.0f );
         
         gocHienTai += gocBuoc;
         buoc++;
      }
      
      // ----
      fprintf( tep, "</svg>\n" );
      // ---- đóng tệp
      fclose( tep );
   }
}


void veDuongCongGiuaBonHinhElip() {
   
   float elip0_gocX = 889.0f;
   float elip0_gocY = 323.5f;
   float elip0_banKinhX = 48.0f;
   float elip0_banKinhY = 28.5f;
   
   float elip1_gocX = 879.4f;
   float elip1_gocY = 209.8f;
   float elip1_banKinhX = 57.6f;
   float elip1_banKinhY = 34.2f;
   
   float elip2_gocX = 775.0f;
   float elip2_gocY = -40.8125f;
   float elip2_banKinhX = 162.0f;
   float elip2_banKinhY = 96.1875f;
   
   float elip3_gocX = 613.0f;
   float elip3_gocY = -280.375f;
   float elip3_banKinhX = 324.0f;
   float elip3_banKinhY = 192.375f;
   
   // ---- tính trung tâm
   float elip0_trungTamX = elip0_gocX + elip0_banKinhX;
   float elip0_trungTamY = elip0_gocY + elip0_banKinhY;
   
   float elip1_trungTamX = elip1_gocX + elip1_banKinhX;
   float elip1_trungTamY = elip1_gocY + elip1_banKinhY;
   
   float elip2_trungTamX = elip2_gocX + elip2_banKinhX;
   float elip2_trungTamY = elip2_gocY + elip2_banKinhY;
   
   float elip3_trungTamX = elip3_gocX + elip3_banKinhX;
   float elip3_trungTamY = elip3_gocY + elip3_banKinhY;
   printf( "trung tâm 0 (%5.1f; %5.1f)\n   trung tâm 1 (%5.1f; %5.1f) trung tâm 2 (%5.1f; %5.1f)\ntrung tâm 3 (%5.1f; %5.1f)\n", elip0_trungTamX, elip0_trungTamY, elip1_trungTamX, elip1_trungTamY, elip2_trungTamX, elip2_trungTamY, elip3_trungTamX, elip3_trungTamY );
   
   // ---- mở tập tin SVG
   FILE *tep = fopen( "netDuongCongGiuaBonHinhElip.svg", "w" );
   
   if( tep != NULL ) {
      
      // ---- đầu tập tin SVG
      unsigned int beRong = 1400;
      unsigned int beCao = 1000;
      fprintf( tep, "<svg version=\"1.1\" width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n\n", beRong, beCao );
      
      // ---- số lượng bước
      unsigned short soLuongBuoc = 20;
      
      float gocBuoc = kPI*2.0f/(float)soLuongBuoc;
      
      unsigned short buoc = 0;
      float gocHienTai = 2.0*kPI/10.0f;
      
      float mangDiemCong[6];
      
      while( buoc < soLuongBuoc ) {
         
         // ---- vị trí
         float cosGocHienTai = cosf( gocHienTai );
         float sinGocHienTai = sinf( gocHienTai );

         float diem0_x = elip0_trungTamX + elip0_banKinhX*cosGocHienTai;
         float diem0_y = elip0_trungTamY + elip0_banKinhY*sinGocHienTai;
         
         float diem1_x = elip1_trungTamX + elip1_banKinhX*cosGocHienTai;
         float diem1_y = elip1_trungTamY + elip1_banKinhY*sinGocHienTai;
         
         float diem2_x = elip2_trungTamX + elip2_banKinhX*cosGocHienTai;
         float diem2_y = elip2_trungTamY + elip2_banKinhY*sinGocHienTai;
      
         float diem3_x = elip3_trungTamX + elip3_banKinhX*cosGocHienTai;
         float diem3_y = elip3_trungTamY + elip3_banKinhY*sinGocHienTai;

         // ---- màu
 //        float giaTri = 128.0f*(0.5f + 0.5f*sinf( gocHienTai ));
//         unsigned int hieuUngSang = floorf( giaTri );
         
         mangDiemCong[0] = diem1_x;
         mangDiemCong[1] = diem1_y;
         mangDiemCong[2] = diem2_x;
         mangDiemCong[3] = diem2_y;
         mangDiemCong[4] = diem3_x;
         mangDiemCong[5] = diem3_y;

         // ---- vẽ nét
         duongCong( tep, diem0_x, diem0_y, mangDiemCong, 1, 0, 0x00, 0.0f, 1, 1.5f, 0x00ff00, 1.0f );
         gocHienTai += gocBuoc;
         buoc++;
      }
      
      // ----
      fprintf( tep, "</svg>\n" );
      // ---- đóng tệp
      fclose( tep );
   }
}

int main( ) {
 
//   taoTuGiac();   // <---- cho tô màu tư giác

//   taoHinhTron();   // <---- cho vẽ hình tròn màu

//  veNetGiuaHaiHinhElip();   // <---- cho vẽ nét giữa hai hình elip
   
   veDuongCongGiuaBonHinhElip();
   
   return 1;
}
