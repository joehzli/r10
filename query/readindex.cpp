#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef struct {
    uint32_t pos;   
    uint16_t context;
} PosItem;

typedef vector<PosItem *> PostingVector;

typedef struct {
    uint32_t docID;
    uint32_t occurence;
    PostingVector posArray;
} HitItem;


vector<HitItem*> *GetIndex(int fileID, uint32_t pointer, uint32_t occurence) {
	FILE *fp = fopen("data/inverted_0.index", "rb");
	fseek(fp, pointer,0);
	vector<HitItem*> *result = new vector<HitItem*>();
	for(int i= 0;i<occurence;i++) {
		HitItem *hitItem = new HitItem;
		fread(&hitItem->docID, sizeof(uint32_t), 1, fp);
		fread(&hitItem->occurence, sizeof(uint32_t), 1, fp);
		cout<<"docID, occurence:"<<hitItem->docID<<", "<<hitItem->occurence<<endl;
		for(int j=0;j<hitItem->occurence;j++) {
			PosItem *posItem = new PosItem;
			fread(&posItem->pos, sizeof(uint32_t), 1, fp);
			fread(&posItem->context, sizeof(uint16_t), 1, fp);
			hitItem->posArray.push_back(posItem);
			cout<<"pos, context:"<<posItem->pos<<", "<<posItem->context<<endl;
		}
		result->push_back(hitItem);
	}
	return result;
}
int main() {
	vector<HitItem*> *result = GetIndex(0, 97964104,361);
	cout<<result->size()<<endl;
	//cout<<sizeof(unsigned long)<<" "<<sizeof(uint32_t)<<endl;
	//print len(l)
	//print l[0].docID, l[0].occurence, l[0].poslist[0].pos, l[0].poslist[0].context
	return 0;
}