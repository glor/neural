typedef struct {
	int depth;
	int* widths;
	double** weights;
} network;

network* make_network(int depth, int* widths) {
	network* nw = malloc(sizeof(nw));
	nw->weights = malloc(depth*sizeof(double*));
	nw->widths = malloc(depth*sizeof(int));
	for(int i=0; i<depth; i++) {
		nw->width[i] = width[i];
	}
	for(int i=1; i<depth; i++) {
		nw->weights[i-1] = malloc(widths[i-1]*widths[i]*sizeof(double));
	}
	return nw;
}

double get_weight(network* nw, int layer, int from, to) {
	return nw->weights[layer][i*nw->width[]]
}
