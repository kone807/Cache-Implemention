#include <bits/stdc++.h>
using namespace std;
#define pb push_back

//upper limit -> fff
// use bitset<bits>(n) to print n in binary
static int hits = 0;
class Block
{
public:
    int arr[1024] = {0}; //a, b, c, d, id
    int id=-1;
    int set = -1;
    
};

class Set
{
public:
    vector<Block> input;
    queue<int> pending;
    int c = 0;
};

void NWSsearch(Block main_memory[], int mlines, Set cache_memory[], int clines, int address, int n, int num_words)
{
    int offset = address%num_words;
    int block_number = address/num_words;
    
    Block needed = main_memory[block_number];
    int set_num = needed.set;
    
    for(int i=0; i<cache_memory[set_num].input.size(); i++)
    {
        if(cache_memory[set_num].input[i].id == needed.id){
            cout<<endl<<"CACHE HIT! Value at address "<<hex<<address<<" is "; cout<<dec<<needed.arr[offset];
            hits++;
            return;
        }
    }
    
    {
        cout<<endl<<"CACHE MISS! Importing address from main memory now!";
        
        cache_memory[set_num].pending.push(cache_memory[set_num].c++); // push counter into the required set's queue
        
        int index = cache_memory[set_num].pending.front()%n;
        
        cache_memory[set_num].pending.pop();
        
        cache_memory[set_num].input[index] = needed;
        
        cout<<endl<<"Value loaded into cache! Value at address "<<hex<<address<<" is "; cout<<dec<<needed.arr[offset];
    }
}


void NWSwrite(Block main_memory[], int mlines, Set cache_memory[], int num_sets, int address, int value, int n, int num_words)
{
    int offset = address%num_words;
    int block_number = address/num_words;
    
    Block needed = main_memory[block_number];
    int set_num = needed.set;
    
    for(int i=0; i<cache_memory[set_num].input.size(); i++)
    {
        if(cache_memory[set_num].input[i].id == needed.id){
            
            cout<<endl<<"CACHE HIT! Value Written!";
            hits++;
            main_memory[block_number].arr[offset] = value;
            cache_memory[set_num].input[i].arr[offset] = value;
            return;
        }
    }
    
    {
        cout<<endl<<"CACHE MISS! Address will be loaded into the cache and value will be written";
        
        cache_memory[set_num].pending.push(cache_memory[set_num].c++); // push counter into the required set's queue
        
        int index = cache_memory[set_num].pending.front()%n;
        
        cache_memory[set_num].pending.pop();
        
        cache_memory[set_num].input[index] = needed;
        
        main_memory[block_number].arr[offset] = value;
        cache_memory[set_num].input[index].arr[offset] = value;
        
    }
} 

void printMainMemory(Block main_memory[], int mlines, int num_words)
{
    cout<<endl<<"  ----------------------------------------------------------------------------------------------------------"<<endl;
    
    cout<<"  |  "<<"Block"<<" |  "<<"                               Address => Value Pairs (hex => dec)                           |";
    
    cout<<endl<<"  ----------------------------------------------------------------------------------------------------------"<<endl;
    
    for(int i=0; i<mlines; i++)
    {
        
        cout<<"  |  "<<setfill('0')<<setw(4)<<i;
        
        for(int j=0; j<4; j++)
        {                                                                           
            int address = i*4 + j;
            cout<<"  |  "<<hex<<setfill('0')<<setw(4)<<address<<" => "<<dec<<setfill('0')<<setw(10)<<main_memory[i].arr[j];
        }
        cout<<"  |"<<endl<<"  ----------------------------------------------------------------------------------------------------------"<<endl;
        
    }
}

void printCacheMemory(Set cache_memory[], int clines, int num_sets, int num_words)
{
    cout<<endl<<"---------------------------------------------------------------------------------------------------------------------------"<<endl;
    
    cout<<"|Set"<<"|Line| "<<"Block"<<"|"<<"                               Address => Value Pairs (hex => dec)                           ";
    
    cout<<endl<<"---------------------------------------------------------------------------------------------------------------------------"<<endl;
    
    int line_number = 0;
    for(int i=0; i<num_sets; i++)
    {   
        for(int k=0; k<cache_memory[i].input.size(); k++){
        cout<<"|"<<setfill('0')<<setw(3)<<i<<"|"<<setfill('0')<<setw(4)<<line_number++<<"| "<<setfill('0')<<setw(4)<<cache_memory[i].input[k].id;
            
        for(int j=0; j<num_words; j++)
        {    
            Block b = cache_memory[i].input[k];
            
            int address = j + b.id*num_words;
            cout<<" | "<<hex<<setfill('0')<<setw(3)<<address<<"=>"<<dec<<setfill('0')<<setw(4)<<b.arr[j];
        }
        cout<<" |"<<endl<<"---------------------------------------------------------------------------------------------------------------------------"<<endl;
        }  
    }
}

int main()
{
    // file reading
    queue<string> input;
    
    ifstream fin;
    fin.open("nwstest.txt");
    string line; 
    
    while(fin)
    {
        getline(fin,line);
        if(line=="-1")
            break;
        input.push(line);
    }
    
    cout<<"Enter 1 for manual run, 0 for run through text file: ";
    int f; cin>>f;
    
    //it is assumed 1 block contains 4 words, therefore, offset size is 2, length of 1 word varies
    int m, c, b, n;
    cout<<"Enter main memory size: "; 
    if(f)cin>>m; 
    else{m=stoi(input.front()); input.pop();}
    
    cout<<endl<<"Enter cache size: "; 
    if(f)cin>>c; else{c=stoi(input.front()); input.pop();}
    
    cout<<endl<<"Enter block size: "; 
    if(f)cin>>b; else{b=stoi(input.front()); input.pop();}
    
    cout<<endl<<"Enter the value of 'n': "; 
    if(f)cin>>n; else{n=stoi(input.front()); input.pop();}
    
    cout<<endl;
    
    int mlines = m/b, clines = c/b, num_words = c/b, num_sets = clines/n; 
    
    // n denotes number of blocks in 1 set
   
   //initializing main memory and cache memory
    
    Block main_memory[mlines];
    for(int i=0; i<mlines; i++)
    {
        Block b;
        
        b.id = i;
        b.set = i%num_sets;
        
        main_memory[i] = b;
    }
    
    Set cache_memory[num_sets];
    
    for(int i=0; i<clines; i++)
    {
        Block b;
      
        b.id = i;
        b.set = i%num_sets; 
    
       cache_memory[b.set].input.pb(b);
       cache_memory[b.set].pending.push(cache_memory[b.set].c++);
    }
    
    cout<<endl<<"Initial Layout of cache memory: "<<endl;
    
  //  printMainMemory(main_memory, mlines, num_words);
    printCacheMemory(cache_memory, clines, num_sets, num_words);
    
    // instruction format -> read add || write add val
 
    int q;
    cout<<endl<<"Enter Total Number of queries: "; 
     if(f)cin>>q; else{q=stoi(input.front()); input.pop();}
    int temp = q;
    
    while(temp--){
        
       string instruction;
    cout<<endl<<"Enter desired instruction (Read/Write val): "; 
    if(f)cin>>instruction; else{instruction=input.front(); input.pop();}
    
    transform(instruction.begin(), instruction.end(), instruction.begin(), ::tolower);
    
    if(instruction.substr(0,4) == "read")
    {
        int address; 
         if(f)cin>>hex>>address; else{address=stoi(input.front(),0,16); input.pop();}
        
        if(address>=m)
        {
            cout<<endl<<"Address does not exist";
            return 0;
        }
        NWSsearch(main_memory, mlines, cache_memory, clines, address, n, num_words);
        
    } 
    
    
    
   else if(instruction.substr(0,5) == "write")
   {
       int address, value; 
       if(f)cin>>hex>>address>>dec>>value; else{address=stoi(input.front(),0,16); input.pop(); value=stoi(input.front()); input.pop();}
       
       if(address>=m)
       {
           cout<<endl<<"Address does not exist";
           return 0;
           
    }
       NWSwrite(main_memory, mlines, cache_memory, clines, address, value, n, num_words);
      
    }
    
    else
    {
        cout<<endl<<"Undefined Instruction!";
        return 0;
    }
   
   //print cache memory after each query
    printCacheMemory(cache_memory, clines, num_sets, num_words);
    }
    
    cout<<endl<<"Final layout of cache memory: "<<endl;
  //  printMainMemory(main_memory, mlines, num_words); cout<<endl;
    printCacheMemory(cache_memory, clines, num_sets, num_words);
    
    double hit_ratio = double(hits)/double(q);
    cout<<endl<<"Total hits: "<<hits<<endl<<"Total misses: "<<q-hits<<endl<<"HIT Ratio: "<<hit_ratio<<endl;
     
    return 0;
}
