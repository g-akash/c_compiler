struct abc{
	int a;
	float f[10];
};

struct cd{
	float a;
	struct abc ha[10];
};

struct nn{
	int a;
	float g;
};


struct cd f(struct abc a,float a1[10],struct nn n1[2]){
	struct cd o;
	int i;
	for(i=0;i<10;i++){
		o.ha[i] = a;
		o.a = n1[i/5].g + a1[i]*i;
	}
	return o;
}

int main(){
	struct abc a;
	struct cd cc;
	float ff[10];
	int i,j,k;
	struct nn hello[2];
	for(i=0;i<10;i++){
		a.f[i] = 3*i;
		ff[i] = 12.12*i-i/2;
		hello[i/5].a=i;
		hello[i/5].g=7*i;
	}
	cc=f(a,ff,hello);
	for(i=0;i<10;i++){
		printf(cc.ha[i].f[i]);
		// printf("%f\n",cc.ha[i].f[i]);
	}

}
