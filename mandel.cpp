#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <GL/glut.h>
#include <mpi.h>


#define rmin   -2.0
#define rmax   2.0
#define imin   -2.0
#define imax   2.0

#define MaxIter 100

#define height  500
#define width   500


#define NumColor 16

int kresult[500][500]={0};
typedef struct complextype{
        float real, imag;
} Compl;

void DrawPoints(float x, float y, int k)
{
    switch (k%NumColor){
    case 0:
        glColor3ub(255,255,255); //WHITE
        break;
    case 1:
        glColor3ub(0,0,0);       //BLACK
        break;
    case 2:
        glColor3ub(255,0,0);       //RED
        break;
    case 3:
        glColor3ub(255,255,0);       //YELLOW
        break;
    case 4:
        glColor3ub(0,255,0);       //GREEN
        break;
    case 5:
        glColor3ub(0,255,255);       //CYAN
        break;
    case 6:
        glColor3ub(0,0,255);       //BLUE
        break;
    case 7:
        glColor3ub(255,0,255);       //MAGNETA
        break;
    case 8:
        glColor3ub(255,128,255);       //AQUAMARINE
        break;
    case 9:
        glColor3ub(0,128,0);       //FORESTGREEN
        break;
    case 10:
        glColor3ub(200,128,0);       //ORANGE
        break;
    case 11:
        glColor3ub(200,0,255);       //MAROON
        break;
    case 12:
        glColor3ub(128,128,64);       //BROWN
        break;
    case 13:
        glColor3ub(255,128,128);       //PINK
        break;
    case 14:
        glColor3ub(255,255,128);       //CORAL
        break;
    case 15:
        glColor3ub(128,128,128);       //GRAY
        break;
    }
    glVertex2f(x,y);

}

void DrawMandelbrot()
{
    float DCI = (float)((imax-imin)/height);
    float DCR = (float)((rmax-rmin)/width);
    glBegin(GL_POINTS);
    for (int p=0;p<500;p++)
    {
        for (int q=0;q<500;q++)
        {
            DrawPoints(((float)imin+(float)p*DCR)/2.0,-((float)rmin+(float)q*DCR)/2.0,kresult[p][q]);
        }
    }
    glEnd();
    glFlush();
}

int main(int argc,char **argv)
{
    int rank,procs;
    int data_tag=1;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    double start_time,end_time,exec_time;
    int i,j,k;
    Compl z, c;
    float lengthsq, temp;
    float DCI = (float)((imax-imin)/height);
    float DCR = (float)((rmax-rmin)/width);

    if(rank==0)
    {
	start_time=MPI_Wtime();
    }


    for(i=0;i<width;i++)
    {
        if(rank==0)
        {
                int index,m;
                int ktemp[500]={0};
                for(index=1;index<procs;index++)
                {
                        MPI_Recv(ktemp,500,MPI_INT,index,data_tag,MPI_COMM_WORLD,&status);
                        for(m=index-1;m<500;)
                        {
                                kresult[i][m]=ktemp[m];
                                m+=procs-1;
                        }
                }
        }
        else
        {
            int ktemp[500]={0};

            for(j=rank-1;j<height;)
            {
                z.real = z.imag = 0.0;
                c.real = (float)rmin + ((float)i*DCR);
                c.imag = (float)imin + ((float)j*DCR);
                k = 0;
                do{
                        temp = z.real*z.real - z.imag*z.imag + c.real;
                        z.imag = (float)2.0*z.real*z.imag + c.imag;
                        z.real = temp;
                        lengthsq = z.real*z.real+z.imag*z.imag;
                        k++;
                } while (lengthsq < 4.0 && k < MaxIter);
                ktemp[j]=k;
                j+=procs-1;
                }
                MPI_Send(ktemp,500,MPI_INT,0,data_tag,MPI_COMM_WORLD);
		}
	}

	if(rank==0)
        {
            end_time=MPI_Wtime();
            exec_time=end_time-start_time;
            printf("%f\n",exec_time);
        }
	

        if (rank==0)
        {
            glutInit(&argc,argv);
            glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
            glutInitWindowSize(500,500);
            glutCreateWindow("Test");
            glutDisplayFunc(&DrawMandelbrot);
            //glutMainLoop();  //Uncomment this line to see the result.
        }


        MPI_Finalize();
	return 0;
}   
