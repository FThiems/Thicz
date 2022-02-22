#include <iostream>
#include "tgaimage.h"
#include "model.h"

const TGAColor white    = TGAColor(254, 255, 255, 255);
const TGAColor red      = TGAColor(255, 0,   0,   255);
const TGAColor blue     = TGAColor(0, 0,   255,   255);
const TGAColor green    = TGAColor(0, 255, 0,     255);
const TGAColor magenta  = TGAColor(255, 0,   255, 255); 
Model *model = NULL;
const int width = 800;
const int height = 800;


//Algo de Bresenham pour dessiner une ligne
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
void triangle(float Ax, float Ay, float Az, float Bx, float By, float Bz, float Cx, float Cy, float Cz, TGAImage &image, float intensity, float* zbuffer){
        float HGx = std::min(std::min(Ax,Bx), Cx);
        float HGy = std::max(std::max(Ay,By), Cy);
        float BGx = std::max(std::max(Ax,Bx), Cx);
        float BGy = std::min(std::min(Ay,By), Cy);

	float u,v,w,z;
        for (int x = HGx; x<=BGx; x++ ){
                for (int y=HGy; y>=BGy; y--){
			//Au lieu de passer par les matrices je passe par la résoltion du sytème à 3 équations pour les coo barycentriques
			u = ((By - Cy)*(x-Cx) + (Cx-Bx)*(y-Cy))/((By-Cy)*(Ax-Cx) + (Cx-Bx)*(Ay-Cy));
			v = ((Cy - Ay)*(x-Cx) + (Ax-Cx)*(y-Cy))/((By-Cy)*(Ax-Cx) + (Cx-Bx)*(Ay-Cy));
			w = 1-u-v;
			if ( u> 1 || u < 0 || v > 1 || v < 0 || w>1 || w < 0 )	continue;	
			z = 0;	
			z = Az*u + Bz*v + Cz*w;
			if (zbuffer[(int)(x+y*width)]<z){
				zbuffer[(int)(x+y*width)] = z;
                        	image.set(x,y,TGAColor(intensity*255,intensity*255,intensity*255,255));
			}
                }
        }
}

int main(int argc, char** argv) {
        if (2==argc) {
        	model = new Model(argv[1]);
        } else {
        	model = new Model("obj/african_head.obj");
        }

        TGAImage image(width, height, TGAImage::RGB);
	//Direction de la lumière
	Vec3f light_dir(0,0,-1);
	//Tableau du z-buffer
	float *zbuffer = new float[width*height];

        //triangle(10,50,70,180,200,70,image);
        

        // line(70,130,200,20,image,white);

        // //line(20,50,200,250,image,white);
	// Wireframe rendering
	/**
        for (int i=0; i<model->nfaces(); i++) { 
        std::vector<int> face = model->face(i); 
                for (int j=0; j<3; j++) { 
                        Vec3f v0 = model->vert(face[j]); 
                        Vec3f v1 = model->vert(face[(j+1)%3]); 
                        int x0 = (v0.x+1.)*width/2.; 
                        int y0 = (v0.y+1.)*height/2.; 
                        int x1 = (v1.x+1.)*width/2.; 
                        int y1 = (v1.y+1.)*height/2.; 
                        line(x0, y0, x1, y1, image, white); 
                } 
        }
	**/

	//Rendering with triangles
	/**
	for (int i = 0; i<model->nfaces(); i++) {
		std::vector<int> face = model->face(i);	
		Vec2i x,y,z;
		Vec3f coo = model->vert(face[1]); 
		x = Vec2i((coo.x+1.)*width/2., (coo.y+1.)*height/2);
		coo = model->vert(face[2]); 
		y = Vec2i((coo.x+1.)*width/2., (coo.y+1.)*height/2);
		coo = model->vert(face[0]); 
		z = Vec2i((coo.x+1.)*width/2., (coo.y+1.)*height/2);
		triangle(x.x, x.y, y.x, y.y, z.x, z.y, image);
	}
	**/

	//BFC
	for (int i = 0; i<model->nfaces(); i++) {
		std::vector<int> face = model->face(i);	
		Vec3f x,y,z;
		Vec3f w_x, w_y, w_z;
		Vec3f coo = model->vert(face[0]); 
		x = Vec3f((coo.x+1.)*width/2., (coo.y+1.)*height/2, coo.z);
		w_x = coo;
		coo = model->vert(face[1]); 
		y = Vec3f((coo.x+1.)*width/2., (coo.y+1.)*height/2, coo.z);
		w_y = coo;
		coo = model->vert(face[2]); 
		z = Vec3f((coo.x+1.)*width/2., (coo.y+1.)*height/2, coo.z);
		w_z = coo;

		Vec3f n = (w_z-w_x)^(w_y-w_x);
		n.normalize();
		float ii = n*light_dir;
		if (ii>0)
			triangle(x.x, x.y, x.z, y.x, y.y, x.z, z.x, z.y, x.z, image, ii, zbuffer);
	}
        // image.set(52, 41, red);
        image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        image.write_tga_file("output.tga");
        return 0;
}

