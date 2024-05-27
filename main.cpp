#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

int read_count=0;
int write_count=0;
int hit=0;
int miss=0;
int eviction=0;
int writeback=0;
int cache_size=0;
int cache_block_size=0;
int cache_block_num=0;
int cache_associativity=0;
int cache_group_num=0;
struct block
{
    bool valid;
    int tag;
    bool dirty;
    int lru;
    block()
    {
        valid=false;
        tag=0;
        dirty=false;
        lru=0;
    }
};
block* cache;
void access(int type,int addr)
{
    if(type==0||type==2)
        read_count++;
    else
        write_count++;
    for(int i=0;i<cache_block_num;i++)
        cache[i].lru++;
    int blocknum=addr/cache_block_size;
    int index0=blocknum%cache_group_num;
    for(int i=0;i<cache_associativity;i++)
    {
        int index=index0*cache_associativity+i;
        if(cache[index].valid&&cache[index].tag==blocknum)
        {
            hit++;
            cache[index].lru=0;
            if(type==1)cache[index].dirty=true;
            return;
        }
    }
    miss++;
    for(int i=0;i<cache_associativity;i++)
    {
        int index=index0*cache_associativity+i;
        if(!cache[index].valid)
        {
            cache[index].valid=true;
            cache[index].tag=blocknum;
            cache[index].lru=0;
            if(type==1)cache[index].dirty=true;
            return;
        }
    }
    eviction++;
    int flag=0;
    int index_evict=0;
    for(int i=0;i<cache_associativity;i++)
    {
        int index=index0*cache_associativity+i;
        if(cache[index].lru>flag)
        {
            flag=cache[index].lru;
            index_evict=index;
        }
    }
    cache[index_evict].tag=blocknum;
    cache[index_evict].lru=0;
    if(cache[index_evict].dirty)
        writeback++;
    if(type==0||type==2)
        cache[index_evict].dirty=false;
    else
        cache[index_evict].dirty=true;
}
int main(int argc, char** argv)
{
    if(argc!=5){
        printf("Usage: CacheSim <trace_input_file> <cache_size> <cache_block_size> <cache_associativity>\n");
        return 0;
    }
    ifstream file(argv[1]);
    if(!file.is_open()){
        printf("Can't open file %s\n",argv[1]);
        return 0;
    }
    cache_size=stoi(argv[2], nullptr, 10);
    cache_block_size=stoi(argv[3], nullptr, 10);
    cache_associativity=stoi(argv[4], nullptr, 10);
    cache_block_num=cache_size/cache_block_size;
    cache_group_num=cache_block_num/cache_associativity;
    cache=new block[cache_block_num];
    string s;
    while(getline(file,s))
    {
        int type=s[0]-'0';
        size_t b=s.rfind(' ');
        int addr=stoi(s.substr(2,b), nullptr, 16);
        access(type,addr);
    }
    cout<<"Read: "<<read_count<<endl;
    cout<<"Write: "<<write_count<<endl;
    cout<<"Hit: "<<hit<<endl;
    cout<<"Miss: "<<miss<<endl;
    cout<<"Eviction: "<<eviction<<endl;
    cout<<"Writeback: "<<writeback<<endl;
    cout<<"Miss Rate: "<<(double)miss/(miss+hit)<<endl;
    return 0;
}