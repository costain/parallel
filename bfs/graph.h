struct Graph* createGraph(int V);
struct AdjListNode* newAdjListNode(int dest);
int getNodeDest(struct AdjListNode* node);
void addEdge(struct Graph* graph, int src, int dest); 
void printGraph(struct Graph* graph);
