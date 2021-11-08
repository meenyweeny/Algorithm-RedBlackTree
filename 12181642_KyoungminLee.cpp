#include<iostream>
#include<string>
#include<cmath>
#include<algorithm>
#include<vector>
using namespace std;
#define red 1 //RBTree의 색 red 정수 1로 define
#define black 0 //RBTree의 색 black 정수 0으로 define
#define nil -1
//Properties -> 1.루트:black 2.leaf:black 3.red의 자식은 black 4.same black depth

class ApplicationInfo {
public:
    int id; //애플리케이션 id (key for redblack tree)
    int color; //redBlack tree에서 이 노드가 어떤 색깔일지 (false = black, true = red라고 정의)
    string appName; //애플리케이션 이름
    int byte; //애플리케이션 용량
    int price; //애플리케이션 가격

    ApplicationInfo* parent;
    ApplicationInfo* left;
    ApplicationInfo* right;

    ApplicationInfo() { //기본 생성자 (주로 nil node를 위해 쓰일 것이라 nil을 위한 상태로 초기화)
        id = byte = price = nil;
        color = black;
        appName = "";
        parent = left = right = NULL;
    }

    ApplicationInfo(int id, string appName, int byte, int price, int color) { //생성하는 생성자
        this->id = id;
        this->appName = appName;
        this->byte = byte;
        this->price = price;
        this->color = color;
        parent = left = right = NULL;
    }
};

bool comp(ApplicationInfo* a, ApplicationInfo* b) { //application을 id 기준으로 sort하기 위한 관리함수
    if (a->id < b->id) return true;
    else return false;
}

class managementAppStore {
public:
    ApplicationInfo* root;
    int depthtmp; //결과에서 depth 출력이 필요할 시 사용
    vector<ApplicationInfo*> allApplications; //모든 애플리케이션들의 모음(for updateSalePrice)

    managementAppStore() { //생성자
        root = NULL;
        depthtmp = 0;
    }

    int calculateSalePrice(int price, double p) { //할인가 계산함수
        int saledPrice;
        double tmp;
        tmp = (100 - p) / 100;
        tmp *= price;
        saledPrice = floor(tmp); //할인가의 소수점은 버림
        return saledPrice;
    }

    ApplicationInfo* searchSpecificApplication(int id) { //search 함수 -> 얘가 depth도 찾아줌
        depthtmp = 0; //root의 depth는 0
        ApplicationInfo* tmp = root; //root부터 탐색 시작

        while (tmp != NULL) { //찾을때까지 도는 while문 (혹은 없음을 알 때 까지)
            if (tmp->id == id) { //찾으려는 id와 같으면 그걸 리턴
                return tmp;
            }
            else if (tmp->id > id) { //찾으려는 id가 지금꺼보다 작다면 왼쪽 자식으로 가기
                if (tmp->left->id != nil) {
                    tmp = tmp->left;
                    ++depthtmp;
                }
                else { //왼쪽 자식이 없다면? tmp을 null로 만들어줌 (그럼 null이 리턴)
                    tmp = NULL;
                }
            }
            else { //찾으려는 id가 지금꺼보다 크다면 오른쪽 자식으로 가기
                if (tmp->right->id != nil) {
                    tmp = tmp->right;
                    ++depthtmp;
                }
                else {
                    tmp = NULL;
                }
            }
        }

        return tmp;
    }

    ApplicationInfo* findMyParent(int id) { //insert node시 부모가 될 것을 찾는 함수
        ApplicationInfo* tmp = root; //root부터 시작

        //핵심 -> 원하는 위치에 child가 null이면 그게 부모가 된다고 return하기

        while (1) {
            if (tmp->id > id) { //left child로 가야하는 조건
                if (tmp->left->id == nil) { //left child가 없다? 내가 그의 left child가 되면 된다
                    //그럼 나의 부모가 될 tmp를 return하고 끝
                    break;
                }
                else {
                    tmp = tmp->left; //left child가 있다? 그럼 난 그로부터 또 비교해서 찾아야한다.
                    //그러므로 tmp를 tmp의 left child로 update
                }
            }
            else { //right child로 가야하는 조건의 상태
                //같은 경우는 없다(이 함수를 register에서만 쓸건데, 같은 경우는 이미 처리해줘서 이 함수를 쓰도록 하질 않음)
                if (tmp->right->id == nil) {
                    break;
                }
                else {
                    tmp = tmp->right;
                }
            }
        }

        return tmp;

    }

    bool isDoubleRedState(ApplicationInfo* now) {
        //root면 이 함수를 실행 안해서 parent가 없는 경우는 생각안해도 된다.
        if (now->parent->color == red) return true; //double red가 맞다. -> restructure or recolor
        else return false; //double red가 아니다. -> 그냥 냅둠.
    }

    bool howToRemedy(ApplicationInfo* now) {
        if (now->parent == now->parent->parent->left) { //나의 부모가 조부모의 left node일 때
            if (now->parent->parent->right->color == black) {
                return true; //restructuring하기
            }
            else {
                return false; //recoloring하기
            }
        }
        else {
            if (now->parent->parent->left->color == black) { //내가 부모의 right node일 때
                return true;
            }
            else {
                return false;
            }
        }
    }

    void restructuring(ApplicationInfo* now) {
        ApplicationInfo* myParent = now->parent; //나의 parent
        ApplicationInfo* myGrandParent = myParent->parent; //parent의 parent
        ApplicationInfo* myBigAncestor = myGrandParent->parent; //grandparent의 parent

        //grandparent가 root냐 아니냐에 따라 어디에 연결해줘야하는지가 달라지기 때문에 판단 필요하다

        //1.>일때 -> 방금 insert한 node가 무조건 중간
        if (myParent->left == now && myGrandParent->right == myParent) { //경우에 따라 누가 어디에 위치할지는 이미 알고있다.

            //색 먼저 설정해준다. 방금 insert한 node가 가운데로 솟으므로 그게 black이고 나머지는 red이다.
            now->color = black;
            myParent->color = myGrandParent->color = red;

            if (myGrandParent == root) { //내 조부모가 root일때

                //가운데 node가 될 현재 노드의 left node를 pointer 변경을 통한 설정
                myGrandParent->right = now->left;
                now->left->parent = myGrandParent;

                now->parent = NULL; //root의 부모는 없으므로
                root = now; //가운데 node 새 연결 과정

                //left node의 부모자식 관계로 연결과정
                root->left = myGrandParent;
                myGrandParent->parent = root;

                //가운데 node가 될 현재 노드의 right node 설정과 그에 맞게 subtree 이동하는 과정
                myParent->left = root->right;
                root->right->parent = myParent;
                root->right = myParent;
                myParent->parent = root;
            }
            else { //grandparent가 root가 아닐 때

                myGrandParent->right = now->left;
                now->left->parent = myGrandParent;

                now->parent = myBigAncestor; //증조부모에 연결

                //증조부모의 left, right child 중의 자신의 위치도 판단하여 연결
                if (myBigAncestor->left == myGrandParent) { //내 조부모가 그의 부모의 left child
                    myBigAncestor->left = now;
                }
                else { //내 조부모가 그의 부모의 right child
                    myBigAncestor->right = now;
                }

                //left node의 부모자식 관계로 연결과정
                now->left = myGrandParent;
                myGrandParent->parent = now;

                //가운데 node가 될 현재 노드의 right node 설정과 그에 맞게 subtree 이동하는 과정
                myParent->left = now->right;
                now->right->parent = myParent;
                now->right = myParent;
                myParent->parent = now;
            }
        }

        //2.<일때 -> 방금 insert한 node가 무조건 중간
        else if (myParent->right == now && myGrandParent->left == myParent) { //경우에 따라 누가 어디에 위치할지는 이미 알고있다.

            //색 먼저 설정해준다. 방금 insert한 node가 가운데로 솟으므로 그게 black이고 나머지는 red이다.
            now->color = black;
            myParent->color = myGrandParent->color = red;

            if (myGrandParent == root) { //내 조부모가 root

                //현재 조부모가 방금 insert된 가운데가 될 node의 right가 되어야 하므로 연결과정
                myGrandParent->left = now->right;
                now->right->parent = myGrandParent;

                now->parent = NULL; //root의 부모는 없으므로
                root = now; //insert된 자신을 root로 설정

                //부모자식관계 연결과정
                root->right = myGrandParent;
                myGrandParent->parent = root;

                //가운데 node의 left node 설정과 그에 따른 subtree의 이동
                myParent->right = root->left;
                root->left->parent = myParent;
                root->left = myParent;
                myParent->parent = root;
            }
            else {
                //현재 조부모가 방금 insert된 가운데가 될 node의 right가 되어야 하므로 연결 및 subtree 이동
                myGrandParent->left = now->right;
                now->right->parent = myGrandParent;

                now->parent = myBigAncestor; //증조부모에 연결

                //증조부모의 left, right child 중의 자신의 위치도 판단하여 연결
                if (myBigAncestor->left == myGrandParent) {
                    myBigAncestor->left = now;
                }
                else {
                    myBigAncestor->right = now;
                }

                //부모자식관계 연결과정
                now->right = myGrandParent;
                myGrandParent->parent = now;

                //가운데 node의 left node 설정과 그에 따른 subtree의 이동
                myParent->right = now->left;
                now->left->parent = myParent;
                now->left = myParent;
                myParent->parent = now;
            }
        }

        //3./일때 -> parent가 무조건 중간
        else if (myParent->left == now && myGrandParent->left == myParent) { //경우에 따라 누가 어디에 위치할지는 이미 알고있다.

            //색 먼저 설정해준다
            myParent->color = black;
            now->color = myGrandParent->color = red;

            if (myGrandParent == root) { //내 조부모가 root

                //grandparent의 최종 위치는 right subtree -> 그에 따른 이동 과정
                //parent를 가운데로 만들어 주려면 그에 딸란 subtree의 위치를 재설정 필수
                myGrandParent->left = myParent->right;
                myParent->right->parent = myGrandParent;

                //root가 될 parent node의 위치 설정 과정
                myParent->parent = NULL;
                root = myParent;

                //right subtree만 제대로 설정해주면 된다 (left는 이미 완전히 parent에 붙어서 잘 이동)
                root->right = myGrandParent;
                myGrandParent->parent = root;
            }
            else {

                //grandparent의 최종 위치는 right subtree -> 그에 따른 이동 과정
                //parent를 가운데로 만들어 주려면 그에 딸란 subtree의 위치를 재설정 필수
                myGrandParent->left = myParent->right;
                myParent->right->parent = myGrandParent;

                myParent->parent = myBigAncestor; //증조부모에 연결

                //증조부모의 left, right child 중의 자신의 위치도 판단하여 연결
                if (myBigAncestor->left == myGrandParent) {
                    myBigAncestor->left = myParent;
                }
                else {
                    myBigAncestor->right = myParent;
                }

                //right subtree만 제대로 설정해주면 된다 (left는 이미 완전히 parent에 붙어서 잘 이동)
                myParent->right = myGrandParent;
                myGrandParent->parent = myParent;
            }
        }

        //4.\일떼 -> parent가 무조건 중간
        else if (myParent->right == now && myGrandParent->right == myParent) { //경우에 따라 누가 어디에 위치할지는 이미 알고있다.
            //색 먼저 설정해준다
            myParent->color = black;
            now->color = myGrandParent->color = red;

            if (myGrandParent == root) { //내 조부모가 root

                //현재 삽입된 노드의 최종 위치는 left subtree -> 그에 따른 이동 과정
                //parent를 가운데로 만들어 주려면 그에 딸란 subtree의 위치를 재설정 필수
                myGrandParent->right = myParent->left;
                myParent->left->parent = myGrandParent;

                //root가 될 parent node의 위치 설정 과정
                myParent->parent = NULL;
                root = myParent;

                //left subtree만 제대로 설정해주면 된다 (right는 이미 완전히 parent에 붙어서 잘 이동)
                root->left = myGrandParent;
                myGrandParent->parent = root;
            }
            else {

                //현재 삽입된 노드의 최종 위치는 left subtree -> 그에 따른 이동 과정
                //parent를 가운데로 만들어 주려면 그에 딸란 subtree의 위치를 재설정 필수
                myGrandParent->right = myParent->left;
                myParent->left->parent = myGrandParent;

                myParent->parent = myBigAncestor; //증조부모에 연결

                //증조부모의 left, right child 중의 자신의 위치도 판단하여 연결
                if (myBigAncestor->left == myGrandParent) {
                    myBigAncestor->left = myParent;
                }
                else {
                    myBigAncestor->right = myParent;
                }

                //left subtree만 제대로 설정해주면 된다 (right는 이미 완전히 parent에 붙어서 잘 이동)
                myParent->left = myGrandParent;
                myGrandParent->parent = myParent;

            }
        }

    }

    ApplicationInfo* recoloring(ApplicationInfo* now) { //recoloring 함수
        ApplicationInfo* myParent = now->parent;
        ApplicationInfo* myGrandParent = myParent->parent;
        ApplicationInfo* myUncle;

        //나의 부모가 나의 조부모의 left인지 right인지를 판단 후 uncle을 정해주기
        if (myParent == myGrandParent->left) {
            myUncle = myGrandParent->right;
        }
        else {
            myUncle = myGrandParent->left;
        }

        //grandparent와 나는 red / uncle과 parent는 black로 만들어줌.
        myGrandParent->color = now->color = red;
        myUncle->color = myParent->color = black;

        return myGrandParent; //double red는 recoloring 시 propagate 될 수 있다 -> return하여 재검사
    }


    void registerNewApplication(int id, string appName, int byte, int price) { //애플리케이션 등록 기능
        int color = red; //insert때는 무조건 color Red인 Node로 insert
        ApplicationInfo* tmp = searchSpecificApplication(id); //일단 있는지 찾음

        if (tmp != NULL) { //이미 있는 id를 등록하려하니까 등록 거절
            cout << depthtmp << "\n";
        }
        else {
            tmp = new ApplicationInfo(id, appName, byte, price, color); //받은 정보로 insert할 node 생성
            allApplications.push_back(tmp); //모든 application 가진 vector에 넣기

            ApplicationInfo* leftnil = new ApplicationInfo(); //black color nil node 삽입위한 nil들
            ApplicationInfo* rightnil = new ApplicationInfo();

            if (allApplications.size() == 1) { //initial insert (무조건 root가 된다)
                root = tmp;
                depthtmp = 0;
                root->color = black; //root property 만족시키기 위함

                //nil node 삽입 및 관계 연결 과정
                leftnil->parent = root;
                rightnil->parent = root;
                root->left = leftnil;
                root->right = rightnil;

                cout << "0\n";
            }

            else { //root가 아닐 때

                ApplicationInfo* myParent = findMyParent(id);
                //내가 삽입되어야 할 위치 찾기 -> 즉 나의 parent가 될 사람 찾기
                tmp->parent = myParent;

                if (myParent->id > id) { //내가 부모의 left child가 될 때
                    myParent->left = tmp;
                }
                else { //내가 부모의 right child가 될 때
                    myParent->right = tmp;
                }

                //nil node 삽입 및 관계 연결 과정
                tmp->left = leftnil;
                tmp->right = rightnil;
                leftnil->parent = tmp;
                rightnil->parent = tmp;

                ApplicationInfo* remedy = tmp;
                //double red 상태 check

                while (1) {

                    if (remedy == root) //parent가 root면 끝남 (조부모를 찾을 수 없기 때문)
                        break;

                    if (isDoubleRedState(remedy)) { //double red 상태가 맞으면?
                        //uncle에 따라 restructuring or recoloring
                        //subtree또한 적절히 처리해야한다.

                        if (howToRemedy(remedy)) { //refactor 방법이 restructure일때
                            restructuring(remedy); //restructure 실행
                            break; //실행 후엔 무조건 double red가 아니다 -> while문 탈출
                        }
                        else { //refactor 방법이 recolor일때
                            remedy = recoloring(remedy); //recolor 실행
                            //구조의 변경은 없지만 색의 변경으로 double red propagate 가능성 존재
                            //또 double red state인지 확인하며 while을 돌기 위해 remedy node ptr update
                        }
                    }

                    else { //double red 상태 아니면 끝남
                        break;
                    }
                }

                root->color = black;

                 //root property 만족을 위해 항상 색 바꿔주는 부분
                //(while에서 root까지 propagate 되었을 때 미숙한 처리를 도움)

                tmp = searchSpecificApplication(id); //내가 방금 넣은 id의 애플리케이션 찾기(depth 찾기 위함)
                cout << depthtmp << "\n";
            }
        }

    }

    void searchAndShowApplication(int id) { //애플리케이션 검색 기능

        ApplicationInfo* tmp = searchSpecificApplication(id); //특정 id의 application 찾아 리턴해줌
        //결국 tmp가 지금 내가 찾고싶은 id의 application 정보를 담게 된다.

        if (tmp == NULL) { //검색하는 id에 해당하는 애플리케이션이 없을 때
            cout << "NULL\n";
        }
        else { //검색하는 id에 해당하는 애플리케이션이 있을 때 (depth,이름,용량,가격)
            cout << depthtmp << " " << tmp->appName << " " << tmp->byte << " " << tmp->price << "\n";
        }
    }

    void searchAndUpdateApplication(int id, string name, int byte, int price) { //애플리케이션 업데이트 기능
        ApplicationInfo* tmp = searchSpecificApplication(id); //특정 id의 application 찾아주는 함수
        if (tmp == NULL) { //검색하는 id에 해당하는 애플리케이션이 없을 때
            cout << "NULL\n";
        }
        else { //검색하는 id에 해당하는 애플리케이션이 있을 때 (세가지 정보를 update하고, depth를 출력)
            tmp->appName = name;
            tmp->byte = byte;
            tmp->price = price;
            cout << depthtmp << "\n";
        }
    }

    void applySaledPrice(int rangeFront, int rangeBack, double salePercent) { //애플리케이션 할인 기능
        //sequential search in ordered vs unsorted -> 선택 (시간 복잡도는 O(n)으로 같지만 실제는?)
        sort(allApplications.begin(), allApplications.end(), comp);

        for (auto i : allApplications) {
            if (i->id < rangeFront) continue;
            if (i->id > rangeBack) break;

            i->price = calculateSalePrice(i->price, salePercent); //range에 맞다면 update 수행
        }
    }
};

//global variables for main function
int t; //질의 수
char cmd; //질의 (I,F,R,D)

int main() {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);

    managementAppStore myAppStore; //myAppStore 선언

    cin >> t;

    while (t--) {
        cin >> cmd;
        if (cmd == 'I') { //애플리케이션 등록 기능 수행
            //출력 형식 -> tree에서 애플리케이션 정보가 저장된 노드의 깊이

            //tree에 애플리케이션 등록하고 저장된 깊이 출력
            //만약 이미 있는 application? -> 등록 거절하고 이미 있는 것의 깊이 출력
            int id;
            string appName;
            int byte, price;
            cin >> id;
            cin >> appName;
            cin >> byte >> price;
            myAppStore.registerNewApplication(id, appName, byte, price);
        }
        else if (cmd == 'F') { //애플리케이션 검색 기능 수행
            //애플리케이션 존재하면? 트리에서의 깊이, 애플리케이션 이름, 용량, 가격 출력
            //애플리케이션 존재하지 않으면? NULL 출력

            int id;
            cin >> id;
            myAppStore.searchAndShowApplication(id);

        }
        else if (cmd == 'R') { //애플리케이션 업데이트 기능 수행
            //애플리케이션 존재하면? 정보를 업데이트하고, 그 노드의 깊이 출력
            //존재하지 않으면? NULL 출력
            int id;
            string updateName;
            int updateByte, updatePrice;

            cin >> id;
            cin >> updateName;
            cin >> updateByte >> updatePrice;
            myAppStore.searchAndUpdateApplication(id, updateName, updateByte, updatePrice);

        }
        else if (cmd == 'D') { //애플리케이션 할인 기능 수행
            //범위 rangeFront <= id <= rangeBack 내의 id를 가진 애플리케이션을 모두 탐색 후 가격에 p%의 할인율 적용

            int rangeFront, rangeBack;
            double salePercent;
            cin >> rangeFront >> rangeBack;
            cin >> salePercent;
            myAppStore.applySaledPrice(rangeFront, rangeBack, salePercent);
        }

    }
}
