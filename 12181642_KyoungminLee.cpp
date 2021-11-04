#include<iostream>
#include<string>
using namespace std;

class ApplicationInfo {
public:
    int id; //애플리케이션 id
    string appName; //애플리케이션 이름
    int byte; //애플리케이션 용량
    int price; //애플리케이션 가격
    bool color; //redBlack tree에서 이 노드가 어떤 색깔일지 (false = black, true = red라고 정의)
    
    ApplicationInfo * parent;
    ApplicationInfo * left;
    ApplicationInfo * right;
    
    ApplicationInfo(){ //기본 생성자
        id=byte=price=color=0;
        appName=nullptr;
        parent=left=right=NULL;
    }
    
    ApplicationInfo(int id,string appName,int byte,int price,bool color){ //생성하는 생성자
        this->id = id;
        this->appName = appName;
        this->byte = byte;
        this->price = price;
        this->color = color;
        parent=left=right=NULL;
    }
    
    //필요시 update 기능들 아래에 구현하기
};

class redBlackTree {
public:
    ApplicationInfo * root;
    
    redBlackTree(){
        root = new ApplicationInfo();
    }
    
    void registerNewApplication(int id,string appName, int byte, int price){
        bool color = false;
        ApplicationInfo * tmp = new ApplicationInfo(id,appName,byte,price,color);
        
        //맨아래에 넣고 위치를 찾아야함
        //restructure,recolor 함수 -> 그럼 이걸 할지말지 여부를 판단하는 함수 (double red 판단)
        //특정 Node의 depth 찾아주는 함수 (node에 정보로 넣어줄지 찾을지)
        //특정 node를 찾아주는 함수 (depth찾는거랑 똑같이 id 기준으로 찾기 -> 그로부터 정보를 가져와서 출력가능)
        //특정 node를 찾아주는 함수는, node형을 리턴 -> 정보 출력도 가능이고 업데이트도 가능
        
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
            
        }
        else if(cmd=='D'){ //애플리케이션 할인 기능 수행
            //범위 rangeFront <= id <= rangeBack 내의 id를 가진 애플리케이션을 모두 탐색 후 가격에 p%의 할인율 적용
            int rangeFront,rangeBack;
            double salePercent;
            cin>>rangeFront>>rangeBack;
            cin>>salePercent;
        }
        
    }
}

