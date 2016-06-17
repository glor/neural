#include<stdlib.h>
#include<stdio.h>

typedef struct {
	int depth;
	int* widths;
	double** weights;
} network;

network* make_network(int depth, int* widths) {
	network* nw = malloc(sizeof(nw));
	nw->depth = depth;
	nw->widths = malloc(depth*sizeof(int));
	nw->weights = malloc((depth-1)*sizeof(double*));
	for(int i=0; i<depth; i++) {
		nw->widths[i] = widths[i];
	}
	for(int i=0; i<depth-1; i++) {
		nw->weights[i] = malloc(widths[i]*widths[i+1]*sizeof(double));
	}
	return nw;
}

double getWeight(network* nw, int layer, int from, int to) {
	return nw->weights[layer][from*nw->widths[layer]+to];
}

void setWeight(network* nw, int layer, int from, int to, double weight) {
	nw->weights[layer][from*nw->widths[layer]+to] = weight;
}

void init_network(network* nw) {
	int depth = nw->depth;
	for(int i=0; i<depth-1; i++)
		for(int j=0; j < (nw->widths[i]); j++)
			for(int k=0; k < (nw->widths[i+1]); k++)
				setWeight(nw,i,j,k,1);
}

void init_network2(network* nw) {
	int depth = nw->depth;
	for(int i=0; i<depth-1; i++)
		for(int j=0; j < (nw->widths[i]); j++)
			for(int k=0; k < (nw->widths[i+1]); k++)
				setWeight(nw,i,j,k,  0.5+((double)(rand()%10000))/100000  );
}


//#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
//#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

void eval(network* nw, double* input, double* output) {
	int depth = nw->depth;
	#define clean(n,m) for(int i=0; i<(m); i++) values[(n)][i] = 0;
	int maxwidth = 0;
	for(int i=0; i<depth; i++) {
		maxwidth = maxwidth > nw->widths[i] ? maxwidth : nw->widths[i];
	}
	double values[2][maxwidth];
	clean(0, maxwidth);
	clean(1, maxwidth);
	// copy input into working space
	for(int i=0; i<nw->widths[0]; i++) {
		values[0][i] = input[i];
	}
	
	// evaluate levelwise
	for(int i=0; i<depth-1; i++) {
		// clear nextlevel
		for(int j=0; j<maxwidth; j++) {
			values[i^1][j] = 0;
		}
		for(int j=0; j<nw->widths[i]; j++) {
			for(int k=0; k<nw->widths[i+1]; k++) {
				values[i^1][k] += getWeight(nw,i,j,k)*values[i%2][j];
			}
		}
	}
	for(int i=0; i<nw->widths[depth-1]; i++) {
		output[i] = values[1][i];
	}
}

void backprop(network* nw, double* input, int index, double alpha) {
	int depth = nw->depth;
	#define clean(n,m) for(int i=0; i<(m); i++) values[(n)][i] = 0;
	int maxwidth = 0;
	for(int i=0; i<depth; i++) {
		maxwidth = maxwidth > nw->widths[i] ? maxwidth : nw->widths[i];
	}
	
	double values[depth][maxwidth];
	double values2[depth][maxwidth];
	for(int i=0; i<depth; i++)
		for(int j=0; j<maxwidth; j++)
			values[i][j] = values2[i][j] = 0;
	// copy input into working space
	for(int i=0; i<nw->widths[0]; i++) {
		values[0][i] = input[i];
	}
	// evaluate levelwise
	for(int i=0; i<depth-1; i++) {
		for(int j=0; j<nw->widths[i]; j++) {
			for(int k=0; k<nw->widths[i+1]; k++) {
				values[i+1][k] += getWeight(nw,i,j,k)*values[i][j];
			}
		}
	}
	
	//values2[depth-1][index] = values[depth-1][index];
	values2[depth-1][0] = 0.5;
	values2[depth-1][1] = 0.0;
	values2[depth-1][2] = 0.0;
	
	for(int i=depth-2; i >= 0; i--) {
		for(int j=0; j<nw->widths[i]; j++) {
			for(int k=0; k<nw->widths[i+1]; k++) {
			//*values[i][j] / values[i+1][k]
				int hj = values[i+1][k];
				//double delta = alpha*(values2[i+1][k]-values[i+1][k])*getWeight(nw,i,j,k)*hj*values[i][j];
				values2[i][j] += values[i][j]*getWeight(nw,i,j,k) / values[i+1][k];
				double delta = alpha*(values2[i+1][k]-values[i+1][k])*values[i][j];
				setWeight(nw,i,j,k, getWeight(nw,i,j,k)+delta);
				printf("scaled row %d, nr %d to nr %d by %lf\n", i, j, k, (getWeight(nw,i,j,k)+delta)/getWeight(nw,i,j,k) );
			}
		}
	}
}

void normalize(network* nw) {
	int depth = nw->depth;
	double maxweight = 0;
	for(int i=0; i<depth-1; i++)
		for(int j=0; j < (nw->widths[i]); j++)
			for(int k=0; k < (nw->widths[i+1]); k++)
				maxweight = maxweight > getWeight(nw,i,j,k) ? maxweight : getWeight(nw,i,j,k);
	for(int i=0; i<depth-1; i++)
		for(int j=0; j < (nw->widths[i]); j++)
			for(int k=0; k < (nw->widths[i+1]); k++)
				setWeight(nw,i,j,k, getWeight(nw,i,j,k)/maxweight );
}

void printNetwork(network* nw) {
	int depth = nw->depth;
	printf("===== BEGIN =====\n");
	printf("depth:\n%d\nwidths:\n", depth);
	for(int i=0; i<depth; i++) {
		printf("%d ", nw->widths[i]);
	}
	for(int i=0; i<depth-1; i++) {
		printf("\n");
		printf("Layer %d -> %d:\n", i, i+1);
		for(int j=0; j < (nw->widths[i]); j++) {
			for(int k=0; k < (nw->widths[i+1]); k++)
				printf("%lf ", getWeight(nw,i,j,k));
			printf("\n");
		}
		
	}
	printf("====== END ======\n");
}

int main(void) {
	int depth = 3;
	int width[3] = {3, 3, 3};
	network* nw = make_network(depth, width);
	
	/*int n = 3;
	double weights[3][3] ={	{1,0,1},
							{0,1,0},
							{0,0,1}};
	for(int i=0; i<n; i++) {
		for(int j=0; j<n; j++) {
			setWeight(nw,0,i,j,weights[i][j]);
		}
	}*/
	init_network2(nw);
	/*
	setWeight(nw,0,0,0,1);
	setWeight(nw,0,0,1,0);
	setWeight(nw,0,1,0,0);
	setWeight(nw,0,1,1,1);
	*/
	setWeight(nw,1,1,0,0.5);
	printNetwork(nw);
	double input[3] = {9./6, 2./6, 3./6};
	double output[1];
	
	for(int i=0; i<5; i++) {
		
		eval(nw, input, output);
		for(int i=0; i<3; i++)
			printf("output = %lf ", output[0]);
		printf("\n");
		
		if(output[0] > 0.5) {
			backprop(nw, input, 1, -0.1);
		} else {
			backprop(nw, input, 1, 0.1);
		}
		//normalize(nw);
		printNetwork(nw);
	}
	
}
