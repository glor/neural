
typedef struct {
	int depth;
	int* widths;
	double** weights;
} network;

network* make_network(int depth, int* widths);
double getWeight(network* nw, int layer, int from, int to);
void setWeight(network* nw, int layer, int from, int to, double weight);
void init_network(network* nw);
void eval(network* nw, double* input, double* output);
void backprop(network* nw, double* input, int index, double alpha);
void train(network* nw, double* goal, int count);
void printNetwork(network* nw);
