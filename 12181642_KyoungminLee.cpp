#include<iostream>
#include<string>
#include<cmath>
#include<algorithm>
#include<vector>
using namespace std;
#define red 1 //RBTree의 색 red 정수 1로 define
#define black 0 //RBTree의 색 black 정수 0으로 define

class ApplicationInfo {
public:
    int id; //애플리케이션 id
    string appName; //애플리케이션 이름
    int byte; //애플리케이션 용량
    int price; //애플리케이션 가격
    int color; //redBlack tree에서 이 노드가 어떤 색깔일지 (false = black, true = red라고 정의)
    
    ApplicationInfo * parent;
    ApplicationInfo * left;
    ApplicationInfo * right;
    
    ApplicationInfo(){ //기본 생성자
        id=byte=price=0;
        color=red;
        appName=nullptr;
        parent=left=right=NULL;
    }
    
    ApplicationInfo(int id,string appName,int byte,int price,int color){ //생성하는 생성자
        this->id = id;
        this->appName = appName;
        this->byte = byte;
        this->price = price;
        this->color = color;
        parent=left=right=NULL;
    }
    
    //필요시 update 기능들 아래에 구현하기
};

bool comp(ApplicationInfo * a, ApplicationInfo * b){
    if(a->id<b->id) return true;
    else return false;
}

class redBlackTree {
public:
    ApplicationInfo * root;
    int depthtmp; //결과에서 depth 출력이 필요할 시 사용
    vector<ApplicationInfo *> allApplications; //모든 애플리케이션들의 모음(for updateSalePrice)
    
    redBlackTree(){
        root = new ApplicationInfo();
        depthtmp = 0;
    }
    
    int calculateSalePrice(int price,double p){ //할인가 계산함수
        int saledPrice;
        double tmp;
        tmp = (100-p)/100;
        tmp*=price;
        saledPrice = floor(tmp);
        return saledPrice;
    }
    
    ApplicationInfo * searchSpecificApplication(int id){ //search 함수
        ApplicationInfo * tmp = NULL;
        depthtmp=0;
        
        return tmp;
    }
    
    
    void searchRangeAndUpdate(int rangeFront,int rangeBack,double salePercent){
        
    }
    
    void registerNewApplication(int id,string appName, int byte, int price){ //애플리케이션 등록 기능
        int color = red; //insert때는 무조건 color Red인 Node로 insert
        ApplicationInfo * tmp = searchSpecificApplication(id);
        if(tmp!=NULL){ //이미 있는 id를 등록하려하니까 등록 거절
            cout<<depthtmp<<"\n";
        }
        else{
            tmp = new ApplicationInfo(id,appName,byte,price,color); //받은 정보로 insert할 node 생성
            allApplications.push_back(tmp); //모든 application 가진 vector에 넣기
            //insert 기능 넣기
            //insert할 자리를 찾기
            //insert한 자리에서, parent가 black이면 끝, red면 refactor
            //restructure,recolor의 여부를 uncle의 색을 보고 판단. 판단 후 수행
            //언제까지? root까지 propagate됐거나, parent가 black이거나!
            
            tmp = searchSpecificApplication(id);
            cout<<depthtmp<<"\n";
        }
        
    }
    
    void searchAndShowApplication(int id){ //애플리케이션 검색 기능
        
        ApplicationInfo * tmp = searchSpecificApplication(id); //특정 id의 application 찾아주는 함수
        if(tmp==NULL){ //검색하는 id에 해당하는 애플리케이션이 없을 때
            cout<<"NULL\n";
        }
        else { //검색하는 id에 해당하는 애플리케이션이 있을 때 (depth,이름,용량,가격)
            cout<<depthtmp<<" "<<tmp->appName<<" "<<tmp->byte<<" "<<tmp->price<<"\n";
        }
    }
    
    void searchAndUpdateApplication(int id,string name,int byte,int price){ //애플리케이션 업데이트 기능
        ApplicationInfo * tmp = searchSpecificApplication(id); //특정 id의 application 찾아주는 함수
        if(tmp==NULL){ //검색하는 id에 해당하는 애플리케이션이 없을 때
            cout<<"NULL\n";
        }
        else { //검색하는 id에 해당하는 애플리케이션이 있을 때 (세가지 정보를 update하고, depth를 출력)
            tmp->appName = name;
            tmp->byte = byte;
            tmp->price = price;
            cout<<depthtmp<<"\n";
        }
    }
    
    void applySaledPrice(int rangeFront,int rangeBack,double salePercent){ //애플리케이션 할인 기능
        sort(allApplications.begin(),allApplications.end(),comp);
        for(auto i : allApplications){
            if(i->id < rangeFront) continue;
            if(i->id > rangeFront) break;
            
            i->price = calculateSalePrice(i->price,salePercent);
        }
    }
};


int t; //질의 수
char cmd; //질의 (I,F,R,D)

int main(){
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);

    redBlackTree RBTree;
    
    cin>>t;
    while(t--){
        cin>>cmd;
        if(cmd=='I'){ //애플리케이션 등록 기능 수행
            //출력 형식 -> tree에서 애플리케이션 정보가 저장된 노드의 깊이
            
            //tree에 애플리케이션 등록하고 저장된 깊이 출력
            //만약 이미 있는 application? -> 등록 거절하고 이미 있는 것의 깊이 출력
            int id;
            string appName;
            int byte, price;
            cin>>id;
            cin>>appName;
            cin>>byte>>price;
            RBTree.registerNewApplication(id,appName,byte,price);
        }
        else if(cmd=='F'){ //애플리케이션 검색 기능 수행
            //애플리케이션 존재하면? 트리에서의 깊이, 애플리케이션 이름, 용량, 가격 출력
            //애플리케이션 존재하지 않으면? NULL 출력
            
            int id;
            cin>>id;
            RBTree.searchAndShowApplication(id);
            
        }
        else if(cmd=='R'){ //애플리케이션 업데이트 기능 수행
            //애플리케이션 존재하면? 정보를 업데이트하고, 그 노드의 깊이 출력
            //존재하지 않으면? NULL 출력
            int id;
            string updateName;
            int updateByte, updatePrice;
            
            cin>>id;
            cin>>updateName;
            cin>>updateByte>>updatePrice;
            RBTree.searchAndUpdateApplication(id, updateName, updateByte, updatePrice);
            
        }
        else if(cmd=='D'){ //애플리케이션 할인 기능 수행
            //범위 rangeFront <= id <= rangeBack 내의 id를 가진 애플리케이션을 모두 탐색 후 가격에 p%의 할인율 적용
            
            int rangeFront,rangeBack;
            double salePercent;
            cin>>rangeFront>>rangeBack;
            cin>>salePercent;
            RBTree.applySaledPrice(rangeFront, rangeBack, salePercent);
        }
        
    }
}

