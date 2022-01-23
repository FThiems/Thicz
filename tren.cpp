#include <iostream>
#include "tgaimage.h"
//#include "model.h"

const TGAColor white    = TGAColor(254, 255, 255, 255);
const TGAColor red      = TGAColor(255, 0,   0,   255);
const TGAColor blue     = TGAColor(0, 0,   255,   255);
const TGAColor green    = TGAColor(0, 255, 0,     255);
const TGAColor magenta  = TGAColor(255, 0,   255, 255); 
//Model *model = NULL;
const int width = 300;
const int height = 300;



void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    } 
} 

/**
 * Fonction qui dessine un triangle plein
 */
void triangle(float Ax, float Ay, float Bx, float By, float Cx, float Cy, TGAImage &image){
        float HGx = std::min(std::min(Ax,Bx), Cx);
        float HGy = std::max(std::max(Ay,By), Cy);
        float BGx = std::max(std::max(Ax,Bx), Cx);
        float BGy = std::min(std::min(Ay,By), Cy);

	float M[3][3] = {{float(Ax), Bx, Cx}, {Ay, By, Cy}, {1, 1, 1}};
        float Minv[3][3];
	float determinant = 0;
	float u,v,w;
//Code d'inversion de matrice et de calcul de déterminant
/*
        for(int i = 0; i<3; i++)
                determinant = determinant + (M[0][i] * (M[1][(i+1)%3] * M[2][(i+2)%3] - M[1][(i+2)%3] * M[2][(i+1)%3]));
        std::cout << "det =" << determinant << std::endl; 
        std::cout<<"\n\nInverse of Matrix is: \n";
        for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++)
		{
			
			std::cout<<((M[(j+1)%3][(i+1)%3] * M[(j+2)%3][(i+2)%3]) - (M[(j+1)%3][(i+2)%3] * M[(j+2)%3][(i+1)%3]))/ determinant<<"\t";
			Minv[i][j] = ((M[(j+1)%3][(i+1)%3] * M[(j+2)%3][(i+2)%3]) - (M[(j+1)%3][(i+2)%3] * M[(j+2)%3][(i+1)%3]))/ determinant;	
			std::cout << Minv[i][j] << std::endl; 
		}
                std::cout<<"\n";
        }
*/
        for (int x = HGx; x<=BGx; x++ ){
                for (int y=HGy; y>=BGy; y--){
			//Au lieu de passer par les matrices je passe par la résoltion du ssytème à 3 équations pour les coo barycentriques
/*			u = x * Minv[1][1] + y * Minv[2][1] + 1 * Minv[3][1];
			v = x * Minv[1][2] + y * Minv[2][2] + 1 * Minv[3][2];
			w = x * Minv[1][3] + y * Minv[2][3] + 1 * Minv[3][3];*/
			u = ((By - Cy)*(x-Cx) + (Cx-Bx)*(y-Cy))/((By-Cy)*(Ax-Cx) + (Cx-Bx)*(Ay-Cy));
			v = ((Cy - Ay)*(x-Cx) + (Ax-Cx)*(y-Cy))/((By-Cy)*(Ax-Cx) + (Cx-Bx)*(Ay-Cy));
			w = 1-u-v;
			//std::cout << u << " " << v << " " << w << std::endl;
			if ( u> 1 || u < 0 || v > 1 || v < 0 || w>1 || w < 0 )
				continue;	
                        image.set(x,y,TGAColor(u*255,v*255,w*255,255));
                }
        }
}

int main(int argc, char** argv) {
        // if (2==argc) {
        //         model = new Model(argv[1]);
        // } else {
        //         model = new Model("tinyrenderer/obj/african_head/african_head.obj");
        // }

        TGAImage image(width, height, TGAImage::RGB);

        triangle(10,50,70,180,200,70,image);
        

        // line(70,130,200,20,image,white);

        // //line(20,50,200,250,image,white);
        // for (int i=0; i<model->nfaces(); i++) { 
        // std::vector<int> face = model->face(i); 
        //         for (int j=0; j<3; j++) { 
        //                 Vec3f v0 = model->vert(face[j]); 
        //                 Vec3f v1 = model->vert(face[(j+1)%3]); 
        //                 int x0 = (v0.x+1.)*width/2.; 
        //                 int y0 = (v0.y+1.)*height/2.; 
        //                 int x1 = (v1.x+1.)*width/2.; 
        //                 int y1 = (v1.y+1.)*height/2.; 
        //                 line(x0, y0, x1, y1, image, red); 
        //         } 
        // }

        // image.set(52, 41, red);
        // image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        image.write_tga_file("output.tga");
        return 0;
}

