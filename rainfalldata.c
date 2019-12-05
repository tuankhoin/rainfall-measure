#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <strings.h>

#define HEADER 1
#define MAX_YEAR 3000
#define MONTHS 12
#define PRESENT 2019

#define STAGE2 2
#define STAGE3 3

#define GRAPH 288
#define COLS 24
#define DIV "+-----+----+----+----+----+----+----+----+----+----+----+----+"

/*prototypes*/

typedef struct {
		int location;
		int yy;
		int mm;
		double rain;
		char valid;
} rainfall_t;

char *months[] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

int mygetchar();
void remove_lines(int n);
void error_and_exit(char *err, int line);
int read_data(rainfall_t *data, int maxyear);
int data_checker(rainfall_t *data, int ndata, int month, int year);
double get_rain_val(rainfall_t *data, int ndata, int month, int year);
void s1_print(rainfall_t *data, int ndata, char *months[]);
void month_year_vals(int month, rainfall_t *data, int ndata, char *months[]);
int count(rainfall_t *data, int ndata, int month);
double calc_mean(rainfall_t *data, int ndata, int month);
void s2_print(rainfall_t *data, int ndata, char *months[]);
int sigma(double a, double b);
double calc_tau(int a, int b);
double total_tau(rainfall_t *data, int ndata, int month);
void s3_print(rainfall_t *data, int ndata, char *months[]);
double year_max(rainfall_t *data, int ndata, int year);
int round_up(double a);
void s4_head_print(rainfall_t *data, int ndata, int year);
int bar_val(int year);
void s4_graph_print(rainfall_t *data, int ndata, int year, int scale);
void month_graph(char *months[]);

/*main*/
int main(int argc, char *argv[]){
	
	rainfall_t data[MAX_YEAR];
	int ndata;
	printf("argc=%d\n",argc);
	int a;
	for (a=0; a<argc;a++) {
		printf("argv[%d] = \"%s\"\n",a,argv[a]);
	}
	remove_lines(HEADER);
	ndata= read_data(data, MAX_YEAR);
	
	s1_print(data, ndata, months);
	s2_print(data, ndata, months);
	s3_print(data, ndata, months);
	s4_head_print(data, ndata, 2001);
	s4_graph_print(data, ndata, 2001, 18);
	month_graph(months);
	return 0;
}

/********/
int mygetchar() {
		int c;
		while ((c=getchar())=='\r') {
		}
		return c;
	}

/* print the supplied error message and then exit the program*/
void error_and_exit(char *err, int line) {
	printf("Problem at line %3d: %s\n", line, err);
	exit(EXIT_FAILURE);
}


void remove_lines(int n) {
	int c;
	while ((c=mygetchar()) != EOF) {
		if (c=='\n') {
			n--;
			if (n==0) return;
		}
	}
	/* if get here, something is wrong */
	error_and_exit("Unexpected end of input", __LINE__);
}


int read_data(rainfall_t *data, int maxyear) {
	int location;
	int yy, mm;
	double rain;
	char valid;
	int ndata=0;
	while (scanf("IDCJAC0001,%d,%d,%d,%lf,%c", 
		&location, &yy, &mm, &rain, &valid)==5) {
		if (ndata==maxyear) {
			error_and_exit("Too much input data", __LINE__);
		}
		/* ok to add to the array */
		mygetchar();
		data[ndata].location  = location;
		data[ndata].yy  = yy;
		data[ndata].mm = mm;
		data[ndata].rain = rain;
		data[ndata].valid = valid;
		ndata++;
	}
	
	/* natural end of function, all ok */
	return ndata;
}

int data_checker(rainfall_t *data, int ndata, int month, int year) {
	int k,check;
	check = 0;
	for (k=0 ;k<=ndata-1; k++) {
		if ((data[k].yy==year) && (data[k].mm==month) && (data[k].valid=='Y')) {
			check=1;
			break;
		}
		else if ((data[k].yy==year) && (data[k].mm==month) && (data[k].valid=='N')) {
			check=2;
			break;
		}
	}
	return check;
}

double get_rain_val(rainfall_t *data, int ndata, int month, int year) {
	int k;
	double rainy = 0.0;
	for (k=0 ;k<=ndata-1; k++) {
		if ((data[k].mm==month)&&(data[k].yy==year)) {
			rainy=data[k].rain;
			break;
		}
	}
	return rainy;
}			

void s1_print(rainfall_t *data, int ndata, char *months[]) {
	int i,j;
	int max_year;
	max_year=data[ndata-1].yy;
	printf("S1, site number %06d,%4d datalines in input\n",
		data[0].location, ndata);
	for (i=data[0].yy ; i<=max_year; i++) {
		printf("S1, %d: ",i);
		for (j=1; j<=MONTHS; j++) {
			if (data_checker(data, ndata, j, i)==0) {
			printf(" ... ");
			}
			else if(data_checker(data, ndata, j, i)==1) {
				printf(" %s ", months[j]);
				}                    
			else if(data_checker(data, ndata, j, i)==2) {
				printf(" %s*", months[j]);
			}
		}
		printf("\n");
	}
	printf("\n");
}

void month_year_vals(int month, rainfall_t *data, int ndata, char *months[]) {
	int j,k;
	int count=0;
	int first_year=PRESENT;
	int last_year=0;
	for (j=data[0].yy ; j<=data[ndata-1].yy; j++) {
			for (k=0 ;k<=ndata-1; k++) {
				if ((data[k].yy==j) && (data[k].mm==month)) {
				count++;
				if (data[k].yy < first_year) {
					first_year=data[k].yy;
				}
				if (data[k].yy > last_year) {
					last_year=data[k].yy;
				}	
				break;
				}
			}
	}
	printf("%4d-%4d, ",first_year,last_year);
}

int count(rainfall_t *data, int ndata, int month) {
	int j;
	int count=0;
	for (j=data[0].yy ; j<=data[ndata-1].yy; j++) {
			if (data_checker(data, ndata, month, j)!=0) {
					count++;
				}
			}
	return count;
}

double calc_mean(rainfall_t *data, int ndata, int month) {
	int j;
	int count=0;
	double sum=0.0;
	for (j=data[0].yy ; j<=data[ndata-1].yy; j++) {
			if (data_checker(data, ndata, month, j)!=0) {
					count++;
					sum+=get_rain_val(data, ndata, month, j);
				}
			}
	return sum/count;
}
	
void s2_print(rainfall_t *data, int ndata, char *months[]) {
	int i;	
	for (i=1; i<= MONTHS; i++) {
		printf("S%d, %s, ", STAGE2, months[i]);
		printf("%d values, ",count(data, ndata, i));
		month_year_vals(i, data, ndata, months);
		printf("mean of %3.1lfmm\n", calc_mean(data, ndata, i));
	}
	printf("\n");
}
			
int sigma(double a, double b) {
	if (a<b){
		return 1;
	}
	else if (a>b){
		return -1;
	}
	else {
		return 0;
	}
}

double calc_tau(int a, int b) {
	return a*1.00/(b*1.0*(b-1)/2.0);
}

double total_tau(rainfall_t *data, int ndata, int month) {
	int i,j,n;
	int sum_tau=0;
	double rain_i, rain_j;
	n=count(data, ndata, month);
	for (i=0; i<=n-2; i++) {
		for (j=i+1;j<=n-1;j++) {
			rain_i = get_rain_val(data, ndata, month, 1941+i);
			rain_j = get_rain_val(data, ndata, month, 1941+j);
			sum_tau+=sigma(rain_i, rain_j);
		}
	}
	return calc_tau(sum_tau,n);
}

void s3_print(rainfall_t *data, int ndata, char *months[]) {
	int i;
	for (i=1;i<=MONTHS;i++) {
		printf("S%d, %s, ", STAGE3, months[i]);
		printf("%d values, ",count(data, ndata, i));
		month_year_vals(i, data, ndata, months);
		printf("tau of %1.2lf\n", total_tau(data, ndata, i));
	}
	printf("\n");
}

double year_max(rainfall_t *data, int ndata, int year) {
	int i;
	double max=0.0;
	double n=0.0;
	for (i=1; i<= MONTHS; i++){
		if (data_checker(data, ndata, i, year)!=0) {
			n=get_rain_val(data, ndata, i, year);
			if (n>max) {
				max=n;
			}
		}
	}
	return max;
}

int round_up(double a){
	return a + (a-a/100*100 != 0);
}

void s4_head_print(rainfall_t *data, int ndata, int year) {
	double max;
	max=year_max(data, ndata, year);
	printf("S4, %d ",year);
	printf("max is %.1lf, ",max);
	printf("scale is %d\n", round_up(max/COLS));
}

int bar_val(int year){
	return year-(year/100*100);
}

void s4_graph_print(rainfall_t *data, int ndata, int year, int scale) {
	int avr[COLS*MONTHS]={0};
	int bar[COLS*MONTHS]={0};
	int i,j;
	double avr_height;
	int avr_reach;
	double bar_height;
	int bar_reach;
	
	for (i=1; i<=MONTHS; i++) {
		
		avr_height=calc_mean(data, ndata, i)/scale;
		avr_reach=round_up(avr_height);
		j=avr_reach-1;
		avr[j*MONTHS+i]=1;
		
		bar_height=get_rain_val(data, ndata, i, year)/scale;
		bar_reach=round_up(bar_height);
		if(bar_height!=0){
		for (j=0;j<bar_reach;j++) {
			bar[j*MONTHS+i]=1;
		}
		}
	}
	
	/*print the graph*/
	for (j=COLS-1;j>=0;j--){
		printf("%4d |",scale*(j+1)); 
		for (i=1;i<=MONTHS;i++){
			if ((avr[j*MONTHS+i]==1)&&(bar[j*MONTHS+i]==1)){
			printf(" *%02d*", bar_val(year));
			}
			else if ((avr[j*MONTHS+i]==0)&&(bar[j*MONTHS+i]==1)){
			printf("  %02d ", bar_val(year));
			}
			else if ((avr[j*MONTHS+i]==1)&&(bar[j*MONTHS+i]==0)){
			printf(" ****");
			}
			else {
			printf("     ");
			}
		}
		printf("\n");
	}
	printf("%4d %s\n",0,DIV);
}

void month_graph(char *months[]){
	int i;
	printf("      ");
	for (i=1; i<=MONTHS; i++) {
		printf(" %s ",months[i]);
	}
	printf("\n");
}
	
