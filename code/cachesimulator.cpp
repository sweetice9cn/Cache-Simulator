/*
Cache Simulator
Level one L1 and level two L2 cache parameters are read from file (block size, line per set and set per cache).
The 32 bit address is divided into tag bits (t), set index bits (s) and block offset bits (b)
s = log2(#sets)   b = log2(block size)  t=32-s-b
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>

using namespace std;
//access state:
#define NA 0 // no action
#define RH 1 // read hit
#define RM 2 // read miss
#define WH 3 // Write hit
#define WM 4 // write miss




struct config{
       int L1blocksize;
       int L1setsize;
       int L1size;
       int L2blocksize;
       int L2setsize;
       int L2size;
       };

/* you can define the cache class here, or design your own data structure for L1 and L2 cache
class cache {
      
      }
*/       

struct CacheOrganization{
       int s;
       int b;
       int t;
       int setnum;
       int** TagArray;
       int** ValidBit;
       int** tracker;
       int* LRU;
};

struct CacheIdentification{  //32 bits is the upper bound of these variables
       bitset<32> offset;
       bitset<32> tag;
       bitset<32> index;
};
struct CacheHierarchy{
       CacheOrganization ORG;
       CacheIdentification ID;
};

class Cache{
	int LRU,min;
public:
	int s,b,t,setnum; // s: index bits; b: offset bits; t: tag bits;
	void CacheDefine(int cachesize, int blocksize, int setsize){
    if(setsize == 0){   // fully associative;
      s = 0;
      b = log2(blocksize);
      t = 32 - s - b;
      setnum = cachesize*1024/blocksize;
    }
    else{
  		s = log2(1024*cachesize/blocksize/setsize);
  		b = log2(blocksize);
  		t = 32 - s - b;
    }
	}

	/* Find the mininum element of the tracker array, which is the LRU */
	int LruCal(int** trackerArray, int index, int way){
  		min = trackerArray[index][0]; // initialize 
  		LRU = 0;
	    for (int i = 0;  i < way;  i++){
	        if (trackerArray[index][i] < min && trackerArray[index][i] != 0){
  		        min = trackerArray[index][i];
  		        LRU = i;
  		    }
	    }
	    return LRU;
	}

	int** ArrayDefine2(int index_bits, int col){
		int row = (int)pow(2,index_bits);
		int** array = new int*[row];
		for (int i = 0; i < row; i++){
			array[i] = new int[col];
		}
    return array;
	}

  int* ArrayDefine1(int index_bits){
    int row = (int)pow(2,index_bits);
    int* array = new int[row];
    
    return array;
  }

	void Delete(int** array, int index_bits){
		int row = (int)pow(2,index_bits);
		for(int i = 0; i < row; i++){
        	delete[] array[i];//deletes an inner array of integer;
    	}
    	delete[] array; //delete pointer holding array of pointers;
	}
};

int main(int argc, char* argv[]){

    Cache myCache;
    CacheHierarchy L1, L2;
    config cacheconfig;
    ifstream cache_params;
    string dummyLine;
    cache_params.open(argv[1]);

    while(!cache_params.eof())  // read config file
    {
      cache_params>>dummyLine;                //L1:
      cache_params>>cacheconfig.L1blocksize;  //8 bytes
      cache_params>>cacheconfig.L1setsize;    //1          
      cache_params>>cacheconfig.L1size;       //16KB = 2^14 bytes = 8*1*2048
      cache_params>>dummyLine;                //L2:
      cache_params>>cacheconfig.L2blocksize;  //16 bytes       
      cache_params>>cacheconfig.L2setsize;    //4    
      cache_params>>cacheconfig.L2size;       //32KB = 2^15 bytes = 16*4*512
    }
  
   // Implement by you: 
   // initialize the hirearch cache system with those configs
   // probably you may define a Cache class for L1 and L2, or any data structure you like
  myCache.CacheDefine(cacheconfig.L1size,cacheconfig.L1blocksize,cacheconfig.L1setsize);
  L1.ORG.s = myCache.s;
  L1.ORG.b = myCache.b;
  L1.ORG.t = myCache.t;
  L1.ORG.setnum = myCache.setnum;
  myCache.CacheDefine(cacheconfig.L2size,cacheconfig.L2blocksize,cacheconfig.L2setsize);
  L2.ORG.s = myCache.s;
  L2.ORG.b = myCache.b;
  L2.ORG.t = myCache.t;
  L2.ORG.setnum = myCache.setnum;

  if (cacheconfig.L1setsize != 0){
      /* Define Tag array, Valid Bit array and LRU tracker array */
      L1.ORG.ValidBit = myCache.ArrayDefine2(L1.ORG.s, cacheconfig.L1setsize);
      L1.ORG.TagArray = myCache.ArrayDefine2(L1.ORG.s, cacheconfig.L1setsize);
      L1.ORG.tracker = myCache.ArrayDefine2(L1.ORG.s, cacheconfig.L1setsize);
      L1.ORG.LRU = myCache.ArrayDefine1(L1.ORG.s);
  }
  else{
      L1.ORG.TagArray = myCache.ArrayDefine1(L1.ORG.setnum);
      L1.ORG.ValidBit = myCache.ArrayDefine1(L1.ORG.setnum);
      L1.ORG.LRU = myCache.ArrayDefine1(L1.ORG.setnum);
  }

  if (cacheconfig.L2setsize != 0){
      L2.ORG.ValidBit = myCache.ArrayDefine2(L2.ORG.s, cacheconfig.L2setsize);
      L2.ORG.TagArray = myCache.ArrayDefine2(L2.ORG.s, cacheconfig.L2setsize);
      L2.ORG.tracker = myCache.ArrayDefine2(L2.ORG.s, cacheconfig.L2setsize);
      L2.ORG.LRU = myCache.ArrayDefine1(L2.ORG.s);
  }
  else{
      L2.ORG.TagArray = myCache.ArrayDefine1(L2.ORG.setnum);
      L2.ORG.ValidBit = myCache.ArrayDefine1(L2.ORG.setnum);
      L2.ORG.LRU = myCache.ArrayDefine1(L2.ORG.setnum);
  }



  int L1AcceState = 0; // L1 access state variable, can be one of NA, RH, RM, WH, WM;
  int L2AcceState = 0; // L2 access state variable, can be one of NA, RH, RM, WH, WM;
  int L1Tracker = 1; // tracking # of L1's MRU;
  int L2Tracker = 1; // tracking # of L2's MRU;
  bool L1Store = 0; // store indicator of L1;
  bool L2Store = 0; // store indicator of L2;


   
    ifstream traces;
    ofstream tracesout;
    string outname;
    outname = string(argv[2]) + ".out";
    
    traces.open(argv[2]);
    tracesout.open(outname.c_str());
    
    string line;
    string accesstype;  // the Read/Write access type from the memory trace;
    string xaddr;       // the address from the memory trace store in hex;
    unsigned int addr;  // the address from the memory trace store in unsigned int;        
    bitset<32> accessaddr; // the address from the memory trace store in the bitset;

    if (traces.is_open()&&tracesout.is_open()){    
        while (getline (traces,line)){   // read mem access file and access Cache
            
            istringstream iss(line); 
            if (!(iss >> accesstype >> xaddr)) {break;} //Break when reaching the end of file.
            stringstream saddr(xaddr);
            saddr >> std::hex >> addr;
            accessaddr = bitset<32> (addr);
           
            /* Identification of cache L1 and L2*/
            L1.ID.offset = bitset<32>(accessaddr.to_string().substr(L1.ORG.t + L1.ORG.s, L1.ORG.b));  //offset L1
            L1.ID.index = bitset<32>(accessaddr.to_string().substr(L1.ORG.t, L1.ORG.s));     //index L1
            L1.ID.tag = bitset<32>(accessaddr.to_string().substr(0, L1.ORG.t));      //tag L1
            L2.ID.offset = bitset<32>(accessaddr.to_string().substr(L2.ORG.t + L2.ORG.s, L2.ORG.b));  //offset L2
            L2.ID.index = bitset<32>(accessaddr.to_string().substr(L2.ORG.t, L2.ORG.s));     //index L2
            L2.ID.tag = bitset<32>(accessaddr.to_string().substr(0, L2.ORG.t));      //tag L2
           
            //cout<<L2.ID.index<<endl;
            //cout<<"aaaaaa"<<endl;
            //bitset<32> brk (std::string("00000000000000000000011000000001"));
            bitset<32> brk (std::string("100000000"));

            // if (L1.ID.index.to_ulong() == 256 && L1.ID.tag.to_ulong() ==375767){
            //   cout<<L1.ID.tag.to_ulong()<<' '<<L1.ID.index.to_ulong()<<' '<<L1.ID.offset.to_ulong()<<endl;
            //   cout<<accesstype<<' '<<xaddr<<endl;
            // }
            // if (L2.ID.index.to_ulong() == brk.to_ulong()){
            //   //cout<<"hayyyyyyyyy"<<endl;
            //   cout<<L2.ID.tag.to_ulong()<<' '<<L2.ID.index.to_ulong()<<' '<<L2.ID.offset.to_ulong()<<endl;
            //   cout<<accesstype<<' '<<xaddr<<endl;
              //break;
            // }
            // //bitset<32> brk (std::string("110000000"));
            // if( L1.ID.index.to_ulong() == brk.to_ulong()){
            //   break;
            // }

            // access the L1 and L2 Cache according to the trace;
            if (accesstype.compare("R")==0){    
                //  Implement by you:
                //  read access to the L1 Cache, 
                //  and then L2 (if required), 
                //  update the L1 and L2 access state variable;
              
                /* read hit on L1 */
                if (cacheconfig.L1setsize != 0){
                    for (int wayL1 = 0; wayL1 < cacheconfig.L1setsize; ++wayL1){
                      	if (L1.ID.tag.to_ulong() == L1.ORG.TagArray[L1.ID.index.to_ulong()][wayL1]
                        	 	&& L1.ORG.ValidBit[L1.ID.index.to_ulong()][wayL1] == 1){

                        	 	L1AcceState = 1;
                        	 	L2AcceState = 0;

                        	 	/* updating LRU */
                        	 	L1.ORG.tracker[L1.ID.index.to_ulong()][wayL1] = L1Tracker; //keep on tracking the hit order;
                        	 	L1Tracker++;
                        	 	L1.ORG.LRU[L1.ID.index.to_ulong()] = myCache.LruCal(L1.ORG.tracker, L1.ID.index.to_ulong(), cacheconfig.L1setsize);


                            // L1Lru is the indicator of least recently used block of L1；
                        	 	break;
                      	}
                    }
                }
                else{
                    for (int setnumber = 0; setnumber < L1.ORG.setnum; ++setnumber){
                        if (L1.ID.tag.to_ulong() == L1.ORG.TagArray[setnumber] 
                            && L1.ORG.ValidBit[setnumber] == 1){

                            L1AcceState = 1;
                            L2AcceState = 0;

                            L1Lru[setnumber] = L1Tracker;
                            L1Tracker++;
                            break;
                        }

                    }
                }
                /* Otherwise, read miss on L1 */
                if (L1AcceState != 1){
                    L1AcceState = 2;

                    /* Read miss on L1 but read hit on L2 */
                    if (cacheconfig.L2setsize != 0){
                        for (int wayL2 = 0; wayL2 < cacheconfig.L2setsize; ++wayL2){
                          	if (L2.ID.tag.to_ulong() == L2.ORG.TagArray[L2.ID.index.to_ulong()][wayL2]
                            		&& L2.ORG.ValidBit[L2.ID.index.to_ulong()][wayL2] == 1){

                            		L2AcceState = 1;
                            		/* updating LRU */
                            		L2.ORG.tracker[L2.ID.index.to_ulong()][wayL2] = L2Tracker;
                            		L2Tracker++;
                            		L2.ORG.LRU[L2.ID.index.to_ulong()] = myCache.LruCal(L2.ORG.tracker, L2.ID.index.to_ulong(), cacheconfig.L2setsize);

                              	/* store data in the lowest empty way of L1 */
                                if (cacheconfig.L1setsize != 0){
                            				for (int wayL1 = 0; wayL1 < cacheconfig.L1setsize; ++wayL1){         					
                              					if (L1.ORG.ValidBit[L1.ID.index.to_ulong()][wayL1] == 0){
                                						L1.ORG.TagArray[L1.ID.index.to_ulong()][wayL1] = L1.ID.tag.to_ulong();
                                						L1.ORG.ValidBit[L1.ID.index.to_ulong()][wayL1] = 1;
                                            L1Store = 1; // update store indicator;
                                						/* updating LRU */
                                						L1.ORG.tracker[L1.ID.index.to_ulong()][wayL1] = L1Tracker; // update tracker;
                                      	 		L1Tracker++;
                                      	 		L1.ORG.LRU[L1.ID.index.to_ulong()] = myCache.LruCal(L1.ORG.tracker, L1.ID.index.to_ulong(), cacheconfig.L1setsize);
                                						break;
                              					}
                            				}
                                    /* Otherwise, L1 is full, evict data in L1 according to the LRU policy */
                                    if (L1Store == 0){
                                        L1.ORG.TagArray[L1.ID.index.to_ulong()][L1.ORG.LRU[L1.ID.index.to_ulong()]] = L1.ID.tag.to_ulong();
                                        L1.ORG.ValidBit[L1.ID.index.to_ulong()][L1.ORG.LRU[L1.ID.index.to_ulong()]] = 1;
                                        /* updating LRU */
                                        L1.ORG.tracker[L1.ID.index.to_ulong()][L1.ORG.LRU[L1.ID.index.to_ulong()]] = L1Tracker; 
                                        L1Tracker++;
                                        L1.ORG.LRU[L1.ID.index.to_ulong()] = myCache.LruCal(L1.ORG.tracker, L1.ID.index.to_ulong(), cacheconfig.L1setsize);
                                        break; 
                                    }
                                }
                                else{
                                    for (int setnumber = 0; setnumber < L1.ORG.setnum; ++setnumber){
                                        if (L1.ORG.ValidBit[setnumber] == 0){
                                            L1.ORG.TagArray[setnumber] = L1.ID.tag.to_ulong();
                                            L1.ORG.ValidBit[setnumber] = 1;
                                            L1Store =1;
                                            L1Lru[setnumber] = L1Tracker;
                                            L1Tracker++;
                                            break;
                                        }
                                    }
                                    if (L1Store == 0){
                                        L1.ORG.TagArray[L1Lru] = L1.ID.tag.to_ulong();
                                        L1.ORG.ValidBit[L1Lru] = 1;
                                        L1Lru[L1Lru] = L1Tracker;
                                        L1Tracker++;
                                        break;
                                    }
                                  
                                }
                          	}
                        }                  
                    }
                }

                /* Otherwise, read miss on both L1 and L2 */
                if (L2AcceState != 1 && L1AcceState != 1){
                    L2AcceState = 2;

                    /* store data in the lowest empty way of L2 */
                    if (cacheconfig.L2setsize != 0){  
                    		for (int wayL2 = 0; wayL2 < cacheconfig.L2setsize; ++wayL2){         					
                      			if (L2.ORG.ValidBit[L2.ID.index.to_ulong()][wayL2] == 0){
                        				L2.ORG.TagArray[L2.ID.index.to_ulong()][wayL2] = L2.ID.tag.to_ulong();
                        				L2.ORG.ValidBit[L2.ID.index.to_ulong()][wayL2] = 1;
                                L2Store = 1; // update store indicator;
                        				/* updating LRU */	
                        				L2.ORG.tracker[L2.ID.index.to_ulong()][wayL2] = L2Tracker;
                              	L2Tracker++;
                              	L2.ORG.LRU[L2.ID.index.to_ulong()] = myCache.LruCal(L2.ORG.tracker, L2.ID.index.to_ulong(), cacheconfig.L2setsize);
                        				break;
                    			  }
                    		}
                   				
                    		/* Otherwise, L2 is full, evict data in L2 according to the LRU policy */
                        if (L2Store == 0){
                        		L2.ORG.TagArray[L2.ID.index.to_ulong()][L2.ORG.LRU[L2.ID.index.to_ulong()]] = L2.ID.tag.to_ulong();
                        		L2.ORG.ValidBit[L2.ID.index.to_ulong()][L2.ORG.LRU[L2.ID.index.to_ulong()]] = 1;          		
                        		/* updating LRU */
                        		L2.ORG.tracker[L2.ID.index.to_ulong()][L2.ORG.LRU[L2.ID.index.to_ulong()]] = L2Tracker;
                            L2Tracker++;
                        		L2.ORG.LRU[L2.ID.index.to_ulong()] = myCache.LruCal(L2.ORG.tracker, L2.ID.index.to_ulong(), cacheconfig.L2setsize);
                        }

                        /* Likewise, store data in the lowest empty way of L1 as well */
                    		for (int wayL1 = 0; wayL1 < cacheconfig.L1setsize; ++wayL1){         					
                      			if (L1.ORG.ValidBit[L1.ID.index.to_ulong()][wayL1] == 0){
                        				L1.ORG.TagArray[L1.ID.index.to_ulong()][wayL1] = L1.ID.tag.to_ulong();
                        				L1.ORG.ValidBit[L1.ID.index.to_ulong()][wayL1] = 1;
                                L1Store = 1; // updating store indicator;
                        				/* updating LRU */
                        				L1.ORG.tracker[L1.ID.index.to_ulong()][wayL1] = L1Tracker;
                            	 	L1Tracker++;
                            	 	L1.ORG.LRU[L1.ID.index.to_ulong()] = myCache.LruCal(L1.ORG.tracker, L1.ID.index.to_ulong(), cacheconfig.L1setsize);
                        				break;
                      			}
                    		}
                    			
                    		/* Otherwise, L1 is full, evict data in L1 according to the LRU policy */
                        if (L1Store == 0){
                        		L1.ORG.TagArray[L1.ID.index.to_ulong()][L1.ORG.LRU[L1.ID.index.to_ulong()]] = L1.ID.tag.to_ulong();
                        		L1.ORG.ValidBit[L1.ID.index.to_ulong()][L1.ORG.LRU[L1.ID.index.to_ulong()]] = 1;
                        		/* updating LRU */
                        		L1.ORG.tracker[L1.ID.index.to_ulong()][L1.ORG.LRU[L1.ID.index.to_ulong()]] = L1Tracker; 
                            L1Tracker++;
                            L1.ORG.LRU[L1.ID.index.to_ulong()] = myCache.LruCal(L1.ORG.tracker, L1.ID.index.to_ulong(), cacheconfig.L1setsize);
                        }
                    }
                }
            }

            else {

                //Implement by you:
                // write access to the L1 Cache, 
                //and then L2 (if required), 
                //update the L1 and L2 access state variable;
				
				        /* write hit on L1 */
                for (int wayL1 = 0; wayL1 < cacheconfig.L1setsize; ++wayL1){	
                  	if (L1.ID.tag.to_ulong() == L1.ORG.TagArray[L1.ID.index.to_ulong()][wayL1]
                    	 	&& L1.ORG.ValidBit[L1.ID.index.to_ulong()][wayL1] == 1){

                    	 	L1AcceState = 3;

                    	 	L1.ORG.tracker[L1.ID.index.to_ulong()][wayL1] = L1Tracker;
                    	 	L1Tracker++;
                    	 	L1.ORG.LRU[L1.ID.index.to_ulong()] = myCache.LruCal(L1.ORG.tracker, L1.ID.index.to_ulong(), cacheconfig.L1setsize);

                    	 	/* write through to L2 */
                    	 	/* write hit on L2 */
                    	 	for(int wayL2 = 0; wayL2 < cacheconfig.L2setsize; ++wayL2){
      	                		if (L2.ID.tag.to_ulong() == L2.ORG.TagArray[L2.ID.index.to_ulong()][wayL2]
          	                		&& L2.ORG.ValidBit[L2.ID.index.to_ulong()][wayL2] == 1){

          	                		L2AcceState = 3;

          	                		L2.ORG.tracker[L2.ID.index.to_ulong()][wayL2] = L2Tracker;
          	                		L2Tracker++;
          	                		L2.ORG.LRU[L2.ID.index.to_ulong()] = myCache.LruCal(L2.ORG.tracker, L2.ID.index.to_ulong(), cacheconfig.L2setsize);
          	                		break;
    	                		  }
  	                	  }

    	                	/* Otherwise, write miss on L2 */
                        if (L2AcceState != 3){
        	                	/* write no-allocate, do nothing */
        	                	L2AcceState = 4;
                        }
                        break;
                  	}
                }

                /* Otherwise, write miss on L1 */
                if (L1AcceState != 3){
                    L1AcceState = 4;
                    /* write no-allocate, forward write command to L2 */
                    /* write hit on L2 */
                    for(int wayL2 = 0; wayL2 < cacheconfig.L2setsize; ++wayL2){
      	                if (L2.ID.tag.to_ulong() == L2.ORG.TagArray[L2.ID.index.to_ulong()][wayL2]
          	                && L2.ORG.ValidBit[L2.ID.index.to_ulong()][wayL2] == 1){

          	                L2AcceState = 3;

          	                L2.ORG.tracker[L2.ID.index.to_ulong()][wayL2] = L2Tracker;
          	                L2Tracker++;
          	                L2.ORG.LRU[L2.ID.index.to_ulong()] = myCache.LruCal(L2.ORG.tracker, L2.ID.index.to_ulong(), cacheconfig.L2setsize);
          	                break;
      	                }
                    }

                    /* Otherwise, write miss on both L1 and L2 */
                    if (L2AcceState != 3){
                        /* write no-allocate, do nothing */
                        L2AcceState = 4;
                    }
	              }	            
            }

            tracesout<< L1AcceState << " " << L2AcceState << endl;  // Output hit/miss results for L1 and L2 to the output file;
      
            if (L2.ID.index.to_ulong() == brk.to_ulong()){
              cout<<"["<<L1AcceState<<' '<<L2AcceState<<"]"<<endl;
              
               cout<<L2.ORG.tracker[L2.ID.index.to_ulong()][0]<<' '<<L2.ORG.tracker[L2.ID.index.to_ulong()][1]
               <<' '<< L2.ORG.tracker[L2.ID.index.to_ulong()][2]<<' '<<L2.ORG.tracker[L2.ID.index.to_ulong()][3]<<endl;
               cout<<L2.ORG.LRU[L2.ID.index.to_ulong()]<<endl;
               cout<<endl;
            }


            /* reset asscess states for L1 and L2 to 0 again*/
            L1AcceState = 0;
            L2AcceState = 0;
            /* reset store indicator of L1 and L2 to 0 */
            L1Store = 0;
            L2Store = 0;  
        }
        traces.close();
        tracesout.close(); 

        // need to use the delete[] operator because we used the new[] operator
      	myCache.Delete(L1.ORG.ValidBit,L1.ORG.s);
      	myCache.Delete(L1.ORG.TagArray,L1.ORG.s);
      	myCache.Delete(L1.ORG.tracker,L1.ORG.s);
      	myCache.Delete(L2.ORG.ValidBit,L2.ORG.s);
      	myCache.Delete(L2.ORG.TagArray,L2.ORG.s);
      	myCache.Delete(L2.ORG.tracker,L2.ORG.s);
        delete[] L1.ORG.LRU;
    }
    else cout<< "Unable to open trace or traceout file ";
   
    return 0;
}
