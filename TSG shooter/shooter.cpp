#include <windows.h>
#include<cstdio>
#include<vector>
#include<cmath>
using namespace std;

unsigned int myrand(void){
  static unsigned int y = 3141592;
  y = y ^ (y << 13); y = y ^ (y >> 17);
  return y = y ^ (y << 15);
}

int randi(int d,int u){
	unsigned int x = myrand();
	return x % (u-d) + d;
}

struct pos{
	int y,x;
	pos(int iy,int ix){
		y = iy; x = ix;
	}
	pos(){};
	double dist(pos p){
		return hypot(x-p.x,y-p.y);
	}
};

struct Obj{
	virtual void draw(HDC hdc)=0;
	virtual void update()=0;
	virtual bool valid()=0;
	pos p; double r;
};


int setrange(int v,int a,int b){
	if(v<a)return a;
	if(v>=b)return b;
	return v;
}

struct player : public Obj{
	pos v;
	
	int hp;
	player(pos ip){
		p=ip; v=pos(0,0);
		r = 20;
		hp = 300;
	}
	void draw(HDC hdc){
		Ellipse(hdc,p.x-r,p.y-r,p.x+r,p.y+r);
		TextOut(hdc,p.x-6,p.y-6,TEXT("@"),1);
	}
	void update(){
		p.x += v.x;
		p.y += v.y;
		p.x = setrange(p.x,0,480);
		p.y = setrange(p.y,0,460);
	}
	bool valid(){
		return true;
	}
	void hit(){
		hp--;
	}
};


bool inrange(int v,int a,int b){
	return a<=v && v<=b;
}

struct bullet : public Obj{
	pos v;
	bullet(pos ip, pos iv,double ir){
		p=ip; v=iv; r=ir;
	}
	void draw(HDC hdc){
		Ellipse(hdc,p.x-r,p.y-r,p.x+r,p.y+r);
	}
	void update(){
		p.x += v.x;
		p.y += v.y;
	}
	bool valid(){
		return inrange(p.x,0,480) && inrange(p.x,0,480);
	}
};



struct keystate{
	bool push[256];
	keystate(){
		for(int i=0;i<256;i++){
			push[i]=false;
		}
	}
	void down(WPARAM p){
		push[p] = true;
	}
	void up(WPARAM p){
		push[p] = false;
	}
};

struct WindowProcDef{
	virtual void destroy()=0;
	virtual void paint()=0;
	virtual void update()=0;
	virtual bool isfinish()=0;
	keystate keys;
};


struct shooter : public WindowProcDef{
	POINT pt[3];
	HWND hwnd;
	vector<Obj*> objs;
	int t;
	player* ziki;
	
	char flag[50];
	int lsflag;
	bool start;
	shooter(HWND ihwnd){
		hwnd = ihwnd;
		
		t = 30*60;
		ziki = new player(pos(300,240));
		objs.push_back(ziki);
		
		//const char* fs = "TSG{Y0u_4r3_Pro_0f_E5cap1ng}";
		//lsflag = strlen(fs);
		
		unsigned char fs[] = {
		  0xe0, 0x59, 0x59, 0xd1, 0x39, 0x6d, 0xd0, 0x9b, 0x32, 0x5b, 0xf5, 0xcc,
		  0x6a, 0xe1, 0x64, 0xe6, 0xef, 0xed, 0xea, 0xff, 0x8f, 0x57, 0x81, 0x5c,
		  0xc5, 0xe5, 0xb3, 0x2f
		};
		lsflag = 28;
		memcpy(flag,fs,lsflag);
		
		start=false;
	}
	bool iswin(){
		if(t/60<=0){
			return true;
		}
		return false;
	}
	bool isstart(){
		return start;
	}
	bool isfinish(){
		if(ziki->hp<0 || t/60<=0){
			return true;
		}
		return false;
	}
	void destroy(){
	}
	void paint(){
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd , &ps);
		//printf("objszie %d\n",objs.size());
		for(Obj* x : objs){
			x->draw(hdc);
		}
		char s[100];
		sprintf(s,"HP : %d",ziki->hp);
		TextOut(hdc,10,10,TEXT(s),strlen(s));
		sprintf(s,"Left time : %d",t/60);
		TextOut(hdc,10,30,TEXT(s),strlen(s));
		
		if(isfinish()){
			if(iswin()){
				sprintf(s,"The flag is %s\0",flag);
				TextOut(hdc,100,300,TEXT(s),lsflag+12);
				sprintf(s,"The flag is also written to result_flag.txt");
				TextOut(hdc,100,320,TEXT(s),strlen(s));
				FILE* fp = fopen("result_flag.txt","wb");
				fwrite(flag,1,lsflag,fp);
				fclose(fp);
			}
			else{
				sprintf(s,"You Lose!");
				TextOut(hdc,100,300,TEXT(s),strlen(s));
				sprintf(s,"If you had more HP ...");
				TextOut(hdc,100,320,TEXT(s),strlen(s));
			}
		}
		else if(!isstart()){
			sprintf(s,"Press Space to start");
			TextOut(hdc,150,350,TEXT(s),strlen(s));
			sprintf(s,"Press Arrow key to move");
			TextOut(hdc,150,370,TEXT(s),strlen(s));
		}
		
		EndPaint(hwnd , &ps);
	}
	void update(){
		if(isfinish())return;
		
		{
			//for(int i=37;i<=40;i++)printf("%d ",keys.push[i]);
			//puts("");
			double dx = 0, dy = 0;
			if(keys.push[VK_LEFT])dx = -10;
			else if(keys.push[VK_RIGHT])dx = 10;
			if(keys.push[VK_UP])dy = -10;
			else if(keys.push[VK_DOWN])dy = 10;
			ziki->v.x = dx;
			ziki->v.y = dy;
			//printf("%f %f\n",dx,dy);
			
			if(keys.push[VK_SPACE])start=true;
		}
		
		if(!isstart())return;
		
		for(Obj* x : objs){
			x->update();
		}
		if(t%(t/240+1)==0){
			int k;
			for(k=0;k<3;k++){
				int cy,cx,vy,vx;
				for(;;){
					cy = randi(1,50);
					cx = randi(150,250);
					vy = randi(0,8);
					vx = randi(-8,8);
					if(vx*vx+vy*vy<=100)break;
				}
				//cy=cx=200;
				//vy=vx=10;
				objs.push_back(new bullet(pos(cy,cx),pos(vy,vx),50));
			}
		}
		t--;
		vector<Obj*> tobjs;
		for(Obj* x : objs){
			if(x != ziki){
				flag[(x->p.x % lsflag + lsflag) % lsflag] ^= ((x->p.y + 256) % 256);
			}
			if(x->valid()){
				tobjs.push_back(x);
				//printf("%f\n",x->p.dist(ziki->p));
				if(x != ziki && x->p.dist(ziki->p) < (x->r + ziki->r)){
					//printf("hit %d\n",t);
					ziki->hit();
				}
			}
		}
		
		objs = tobjs;
	}
};


WindowProcDef* proc;


LRESULT CALLBACK WndProc(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp) {
	//printf("msg %d\n",msg);
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		proc->paint();
		return 0;
	case WM_KEYDOWN:
		//printf("down %d\n",wp);
		proc->keys.down(wp);
		return 0;
	case WM_KEYUP:
		//printf("up %d\n",wp);
		proc->keys.up(wp);
		return 0;
	}
	return DefWindowProc(hwnd , msg , wp , lp);
}




#include <sys/time.h>
double getdtime(void){
        timeval tv;
        gettimeofday(&tv, NULL);
        //clock_t nt = clock();
        double res = ((double)(tv.tv_sec) + (double)(tv.tv_usec) * 0.001 * 0.001);
        //printf("nt .. %f\n",res);
        return res;
}

// g++ tes.cpp -lGdi32 -std=c++11 -s -O3 -static-libgcc -static-libstdc++

int WINAPI WinMain(HINSTANCE hInstance , HINSTANCE hPrevInstance ,	PSTR lpCmdLine , int nCmdShow ) {
	
	
	WNDCLASS winc;
	winc.style		= CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc	= WndProc;
	winc.cbClsExtra	= winc.cbWndExtra	= 0;
	winc.hInstance		= hInstance;
	winc.hIcon		= LoadIcon(NULL , IDI_APPLICATION);
	winc.hCursor		= LoadCursor(NULL , IDC_ARROW);
	winc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName	= NULL;
	winc.lpszClassName	= TEXT("MAIN");
	if (!RegisterClass(&winc)){
		fprintf(stderr,"err can't register window\n");
		exit(1);
	}
	
	HWND hwnd = CreateWindowEx(
			WS_EX_COMPOSITED,
			TEXT("MAIN") , TEXT("TSG shooter") ,
			(WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_THICKFRAME ,
			CW_USEDEFAULT , CW_USEDEFAULT ,
			480,480,
			//CW_USEDEFAULT , CW_USEDEFAULT ,
			NULL , NULL , hInstance , NULL
	);
	if (hwnd == NULL){
		fprintf(stderr,"err can't create window\n");
		exit(1);
	}
	
	shooter sh(hwnd);
	proc = &sh;
	MSG msg;
	
	double st = getdtime();
	
	bool isrun = false;
	for(;;){
		if(PeekMessage(&msg , NULL , 0 , 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(msg.message == WM_QUIT)break;
		} else {
			double nt = getdtime();
			if(nt-st > 1.0/60.0){
				st = nt;
				//printf("update\n");
				if(!proc->isfinish()){
					proc->update();
					InvalidateRect(hwnd, NULL,TRUE);
				}
			}
		}
	}
	return 0;
}