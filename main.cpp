#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include <vector>
#include <list>
#include <utility>

using namespace std;

struct address {
    unsigned int x;
    unsigned int y;
    unsigned int n;
    address(int ax, int ay, int an): x(ax), y(ay), n(an) {}
};

struct player_address {
    unsigned int x;
    unsigned int y;
    unsigned int n;
    player_address(int ax, int ay, int gk): x(ax), y(ay), n(gk) {}
};

struct state {
    vector<address> box;
    player_address *player;
    vector<vector<char>> mapi;
    string solution;
    state(vector<address> b, player_address *p, string sol, vector<vector <char> > m): box(b), player(p), solution(sol), mapi(m) {}
};

unsigned int widthGlobal, heightGlobal, boxesGlobal;
vector<address> vectorBoxes;
vector<address> vectorGoals;
vector<vector<char>> mapa; /**< MAPA */
vector<vector <int> > mapa_pola;
int gk=0;

signed int MX[4] = {-1, 0, 1, 0};
signed int MY[4] = {0, -1, 0, 1};

struct states_are_equal :
    public binary_function<state const *, state const *, bool>
{
    bool operator()(state const * a, state const * b) const {
        if(vectorBoxes.size() == 1) {
                return a->player->x == b->player->x &&
               a->player->y == b->player->y &&
               a->box[0].x == b->box[0].x &&
               a->box[0].y == b->box[0].y;
        } else if(vectorBoxes.size() == 2) {
                return a->player->x == b->player->x &&
               a->player->y == b->player->y &&
               a->box[0].x == b->box[0].x &&
               a->box[0].y == b->box[0].y &&
               a->box[1].x == b->box[1].x &&
               a->box[1].y == b->box[1].y;
        } else if(vectorBoxes.size() == 3) {
                return a->player->x == b->player->x &&
               a->player->y == b->player->y &&
               a->box[0].x == b->box[0].x &&
               a->box[0].y == b->box[0].y &&
               a->box[1].x == b->box[1].x &&
               a->box[1].y == b->box[1].y &&
               a->box[2].x == b->box[2].x &&
               a->box[2].y == b->box[2].y;
        } else if(vectorBoxes.size() == 4) {
                return a->player->x == b->player->x &&
               a->player->y == b->player->y &&
               a->box[0].x == b->box[0].x &&
               a->box[0].y == b->box[0].y &&
               a->box[1].x == b->box[1].x &&
               a->box[1].y == b->box[1].y &&
               a->box[2].x == b->box[2].x &&
               a->box[2].y == b->box[2].y &&
               a->box[3].x == b->box[3].x &&
               a->box[3].y == b->box[3].y;
        } else if(vectorBoxes.size() == 5) {
                return a->player->x == b->player->x &&
               a->player->y == b->player->y &&
               a->box[0].x == b->box[0].x &&
               a->box[0].y == b->box[0].y &&
               a->box[1].x == b->box[1].x &&
               a->box[1].y == b->box[1].y &&
               a->box[2].x == b->box[2].x &&
               a->box[2].y == b->box[2].y &&
               a->box[3].x == b->box[3].x &&
               a->box[3].y == b->box[3].y &&
               a->box[4].x == b->box[4].x &&
               a->box[4].y == b->box[4].y;
        } else if(vectorBoxes.size() == 6) {
                return a->player->x == b->player->x &&
               a->player->y == b->player->y &&
               a->box[0].x == b->box[0].x &&
               a->box[0].y == b->box[0].y &&
               a->box[1].x == b->box[1].x &&
               a->box[1].y == b->box[1].y &&
               a->box[2].x == b->box[2].x &&
               a->box[2].y == b->box[2].y &&
               a->box[3].x == b->box[3].x &&
               a->box[3].y == b->box[3].y &&
               a->box[4].x == b->box[4].x &&
               a->box[4].y == b->box[4].y &&
               a->box[5].x == b->box[5].x &&
               a->box[5].y == b->box[5].y;
        } else if(vectorBoxes.size() == 7) {
                return a->player->x == b->player->x &&
               a->player->y == b->player->y &&
               a->box[0].x == b->box[0].x &&
               a->box[0].y == b->box[0].y &&
               a->box[1].x == b->box[1].x &&
               a->box[1].y == b->box[1].y &&
               a->box[2].x == b->box[2].x &&
               a->box[2].y == b->box[2].y &&
               a->box[3].x == b->box[3].x &&
               a->box[3].y == b->box[3].y &&
               a->box[4].x == b->box[4].x &&
               a->box[4].y == b->box[4].y &&
               a->box[5].x == b->box[5].x &&
               a->box[5].y == b->box[5].y &&
               a->box[6].x == b->box[6].x &&
               a->box[6].y == b->box[6].y;
        }
    }
};

template <typename Iterator, typename Comparer>
struct is_in_container_func
{
    is_in_container_func(Iterator begin, Iterator end, Comparer cmp)
        : it_begin(begin), it_end(end), comparer(cmp) { }

    bool operator()(typename std::iterator_traits<Iterator>::value_type const & i) {
        return std::find_if(it_begin, it_end, std::bind1st(comparer, i)) != it_end;
    }

private:
    Iterator it_begin;
    Iterator it_end;
    Comparer comparer;
};

template <typename Iterator, typename Comparer>
is_in_container_func<Iterator, Comparer> is_in_container(
    Iterator begin, Iterator end, Comparer cmp)
{
    return is_in_container_func<Iterator, Comparer>(begin, end, cmp);
}

bool check_solution(vector<address> box, string s) {
    int counter = 0;
    string solution = s;
    for(int i = 0; i<vectorBoxes.size(); i++) {
        if(mapa[box[i].x][box[i].y]=='G') counter++;
    }
    if(counter == box.size()) {
        cout << "hurra! " << "the solution is: " << solution << endl;
        return true;
    }
    else { return false; }
}

bool find_box(vector<address> v, int x, int y, int j) {
    if(v[j].x == x && v[j].y == y) {
        return true;
    }
    else return false;
}

string add_solution_normal(string s, int id) {
    string solution = s;
    if(id == 0) solution.append("u");
    else if(id == 1) solution.append("l");
    else if(id == 2) solution.append("d");
    else if(id == 3) solution.append("r");
    return solution;
}

string add_solution_box(string s, int id) {
    string solution = s;
    if(id == 0) solution.append("U");
    else if(id == 1) solution.append("L");
    else if(id == 2) solution.append("D");
    else if(id == 3) solution.append("R");
    return solution;
}

bool simple_deadlock (vector<vector<char>> given_map, vector<address> given_boxes) {
    //zwraca true jeœli deadlock, false jesli nie
    vector<vector<char>> actual_map = given_map;
    vector<address> boxes = given_boxes;
    int counter = 0;
    bool flaga_gora = false, flaga_lewo = false, flaga_dol = false, flaga_prawo = false;

    for(int j = 0; j<vectorBoxes.size(); j++) {
        for(int i = 0; i < sizeof(MX)/sizeof(MX[i]) ; i++) {

            if(i == 0) {
                for(int k = 0; k<vectorGoals.size(); k++) {
                if((boxes[j].x != vectorGoals[k].x && boxes[j].y != vectorGoals[k].y) &&
                   (actual_map[boxes[j].x + MX[i]][boxes[j].y + MY[i]] == 'X' && (actual_map[boxes[j].x + MX[i+1]][boxes[j].y + MY[i+1]] == 'X' || actual_map[boxes[j].x + MX[i+3]][boxes[j].y + MY[i+3]] == 'X' )))
                        counter++;
                if(counter>=2) { counter = 0; return true; }
                }
            } else if(i == 1) {
                for(int k = 0; k<vectorGoals.size(); k++) {
                if((boxes[j].x != vectorGoals[k].x && boxes[j].y != vectorGoals[k].y) &&
                   (actual_map[boxes[j].x + MX[i]][boxes[j].y + MY[i]] == 'X' && (actual_map[boxes[j].x + MX[i-1]][boxes[j].y + MY[i-1]] == 'X' || actual_map[boxes[j].x + MX[i+1]][boxes[j].y + MY[i+1]] == 'X')))
                        counter++;
                if(counter>=2) { counter = 0; return true; }
                }
            } else if(i == 2) {
                for(int k = 0; k<vectorGoals.size(); k++) {
                if((boxes[j].x != vectorGoals[k].x && boxes[j].y != vectorGoals[k].y) &&
                   (actual_map[boxes[j].x + MX[i]][boxes[j].y + MY[i]] == 'X' && (actual_map[boxes[j].x + MX[i-1]][boxes[j].y + MY[i-1]] == 'X' || actual_map[boxes[j].x + MX[i+1]][boxes[j].y + MY[i+1]] == 'X')))
                        counter++;
                if(counter>=2) { counter = 0; return true; }
                }
            } else if(i == 3) {
                for(int k = 0; k<vectorGoals.size(); k++) {
                if((boxes[j].x != vectorGoals[k].x && boxes[j].y != vectorGoals[k].y) &&
                   (actual_map[boxes[j].x + MX[i]][boxes[j].y + MY[i]] == 'X' && (actual_map[boxes[j].x + MX[i-3]][boxes[j].y + MY[i-3]] == 'X' || actual_map[boxes[j].x + MX[i-1]][boxes[j].y + MY[i-1]] == 'X')))
                        counter++;
                if(counter>=2) { counter = 0; return true; }
                }
            }
        }
    }
}

void generate_children(vector<vector <char> > given_map, player_address *given_player, vector<address> given_box) {
    vector<vector <char> > actual_map;
    vector<vector <char> > original_map;
    int indeks_vector = 0;
    int jot = 0;
    vector<state*> vectorOpen;
    vector<state*> vectorClosed;
    vector<state*> vectorCheck;
    vector<address> vectorBoxesTemp;
    string solution_temp = "";

    state *temp_state;
    state *temp;
    player_address *player_temp;
    int vectorIt = 0;

    player_temp = given_player;
    vectorBoxesTemp = given_box;
    actual_map = given_map;
    original_map = given_map;

    temp_state = new state(vectorBoxesTemp,player_temp,solution_temp,actual_map);

    vectorOpen.insert(vectorOpen.begin(),temp_state);

    while(vectorOpen.size() != 0) {
    //for(int z = 0; z < 8; z++) {
        temp_state = vectorOpen[0];
        temp = vectorOpen[0];
        cout << "Temp state " << temp_state->player->x << "  " << temp_state->player->y;
        for(int k = 0; k<vectorBoxesTemp.size(); k++) {
            cout << " | " << temp_state->box[k].x << "  " <<temp_state->box[k].y;
        }
        cout << endl;
        vectorBoxesTemp = temp_state->box;
        actual_map = vectorOpen[0]->mapi;

        vectorOpen.erase(vectorOpen.begin());

        if(check_solution(vectorBoxesTemp,solution_temp)) exit(0);
        else {
        for(int i = 0; i < sizeof(MX)/sizeof(MX[i]) ; i++) {
        switch(i) {
            case 0:
                indeks_vector = widthGlobal*(-1);
                break;
            case 1:
                indeks_vector = -1;
                break;
            case 2:
                indeks_vector = widthGlobal;
                break;
            case 3:
                indeks_vector = 1;
                break;
            default:
                cout << "boziu kochany co ty robisz xD XD" << endl;
                break;
        }

        /**<  jesli przed graczem bedzie sciana, wyjdz */
        if(temp->mapi[temp->player->x + MX[i]][temp->player->y + MY[i]] == 'X') {
            cout << "sciana   " << endl;
        }
        /**< jesli przed graczem bedzie dostepne pole */
        else if(temp->mapi[temp->player->x + MX[i]][temp->player->y + MY[i]] == '.') {
            for(int k = 0; k<vectorGoals.size(); k++) {
                //check if player was standing on goal, if so put 'G'
                if(temp->mapi[temp->player->x][temp->player->y] == original_map[vectorGoals[k].x][vectorGoals[k].y]) {
                    actual_map [temp->player->x][temp->player->y] = 'G';
                    actual_map[temp->player->x + MX[i]][temp->player->y + MY[i]]='M';
                } else //if not then just put '.' {
                    actual_map [temp->player->x][temp->player->y] = '.';
                    actual_map[temp->player->x + MX[i]][temp->player->y + MY[i]]='M';
                }
            }

            player_temp = new player_address(temp->player->x + MX[i],temp->player->y + MY[i],temp->player->n + indeks_vector);
            solution_temp = add_solution_normal(temp->solution,i);
            temp_state = new state(vectorBoxesTemp,player_temp,solution_temp,actual_map);
            actual_map = temp->mapi;

            vectorCheck.insert(vectorCheck.begin() + vectorIt, temp_state);
            vectorIt++;

            cout << "pole " << temp_state->player->x << "  " << temp_state->player->y;
            for(int k=0; k<vectorBoxesTemp.size(); k++) {
                cout << " | " << temp_state->box[k].x << "  " << temp_state->box[k].y;
            }
            cout << endl << temp_state->solution << endl;

        }
        /**< jesli przed graczem jest pole dostepne pole mety */
        else if(temp->mapi[temp->player->x + MX[i]][temp->player->y + MY[i]] == 'G') {
            actual_map [temp->player->x][temp->player->y] = '.';
            actual_map[temp->player->x + MX[i]][temp->player->y + MY[i]]='M';

            player_temp = new player_address(temp->player->x + MX[i],temp->player->y + MY[i],temp->player->n + indeks_vector);
            solution_temp = add_solution_normal(temp->solution,i);
            temp_state = new state(vectorBoxesTemp,player_temp,solution_temp,actual_map);
            actual_map = temp->mapi;

            vectorCheck.insert(vectorCheck.begin() + vectorIt, temp_state);
            vectorIt++;

            cout << "pole " << temp_state->player->x << "  " << temp_state->player->y;
            for(int k=0; k<vectorBoxesTemp.size(); k++) {
                cout << " | " << temp_state->box[k].x << "  " << temp_state->box[k].y;
            }
            cout << endl << temp_state->solution << endl;

        }
        /**< jesli przed graczem znajduje sie skrzynka J */
        else if (temp->mapi[temp->player->x + MX[i]][temp->player->y + MY[i]] == 'J') {
            if(temp->mapi[temp->player->x + MX[i] + MX[i]][temp->player->y + MY[i]+ MY[i]]=='X' || temp->mapi[temp->player->x + MX[i] + MX[i]][temp->player->y + MY[i]+ MY[i]]=='J') {
                cout << "sciana lub krysztal, nie popchasz" << endl;
            } else if(temp->mapi[temp->player->x + MX[i] + MX[i]][temp->player->y + MY[i]+ MY[i]]=='.' || temp->mapi[temp->player->x + MX[i] + MX[i]][temp->player->y + MY[i]+ MY[i]]=='G') {
                for(int k = 0; k<vectorGoals.size(); k++) {
                    if(temp->mapi[temp->player->x][temp->player->y] == original_map[vectorGoals[k].x][vectorGoals[k].y]) {
                        actual_map [temp->player->x][temp->player->y] = 'G';
                        actual_map[temp->player->x + MX[i]][temp->player->y + MY[i]]='M';
                        actual_map[temp->player->x + MX[i] + MX[i]][temp->player->y + MY[i] + MY[i]]='J';
                    } else {
                        actual_map[temp->player->x][temp->player->y] = '.';
                        actual_map[temp->player->x + MX[i]][temp->player->y + MY[i]]='M';
                        actual_map[temp->player->x + MX[i] + MX[i]][temp->player->y + MY[i] + MY[i]]='J';
                    }
                }

                for(int j = 0; j<vectorBoxes.size(); j++) {
                    if(find_box(temp->box,temp->player->x + MX[i],temp->player->y + MY[i], j)) { jot = j; break; }
                }

                vectorBoxesTemp[jot].x = vectorBoxesTemp[jot].x + MX[i];
                vectorBoxesTemp[jot].y = vectorBoxesTemp[jot].y + MY[i];
                vectorBoxesTemp[jot].n = vectorBoxesTemp[jot].n + indeks_vector;

                player_temp = new player_address(temp->player->x + MX[i],temp->player->y + MY[i],temp->player->n + indeks_vector);
                solution_temp = add_solution_box(temp->solution,i);
                temp_state = new state(vectorBoxesTemp,player_temp,solution_temp,actual_map);
                if(check_solution(temp_state->box,solution_temp)) exit(0);

                if(!simple_deadlock(actual_map,vectorBoxesTemp)) {
                    vectorCheck.insert(vectorCheck.begin() + vectorIt, temp_state);
                    vectorIt++;
                }
                actual_map = temp->mapi;
                vectorBoxesTemp = temp->box;

                cout << "skrzynia " << temp_state->player->x << "  " << temp_state->player->y;
                for(int k = 0; k<vectorBoxesTemp.size();k++) {
                    cout << " | " << temp_state->box[k].x << "  " << temp_state->box[k].y;
                }
                cout << endl << temp_state->solution << endl;


            }
        }

        }
        vectorIt = 0;
        vectorClosed.push_back(temp);
        //vector<state*>::iterator new_end = std::remove_if(vectorCheck.begin(), vectorCheck.end(),is_in_container(vectorOpen.begin(), vectorOpen.end(), states_are_equal()));

        vector<state*>::iterator new_end = std::remove_if(vectorCheck.begin(), vectorCheck.end(),is_in_container(vectorOpen.begin(), vectorOpen.end(), states_are_equal()));
        //cout << "Temp state po push back " << temp->player->x << "   " << temp->player->y << "   " << temp->box[0].x << "   " << temp->box[0].y << "  " << temp->box[1].x << "   " << temp->box[1].y << endl;

        new_end = remove_if(vectorCheck.begin(), new_end,is_in_container(vectorClosed.begin(), vectorClosed.end(), states_are_equal()));
        //cout << "Temp state po push back 2 " << temp->player->x << "   " << temp->player->y << "   " << temp->box[0].x << "   " << temp->box[0].y << "  " << temp->box[1].x << "   " << temp->box[1].y << endl;

        vectorCheck.erase(new_end, vectorCheck.end());
        /*
        for(int k = 0; k<vectorCheck.size(); k++) {
            cout << "Element vectora check " << k << " koordynaty  " << vectorCheck[k]->player->x << "   " << vectorCheck[k]->player->y << "   " << vectorCheck[k]->box[0].x << "   " << vectorCheck[k]->box[0].y << "   " << vectorCheck[k]->box[1].x << "   " << vectorCheck[k]->box[1].y << endl;
        }
        */

        for(int k = 0; k<vectorCheck.size(); k++) {
            //cout << "Element vectora check " << k << " koordynaty  " << vectorCheck[k]->player->x << "   " << vectorCheck[k]->player->y << "   " << vectorCheck[k]->box[0].x << "   " << vectorCheck[k]->box[0].y << endl;
            vectorOpen.push_back(vectorCheck[k]);
            //cout << "Element vectora open " << k << " koordynaty  " << vectorOpen[k]->player->x << "   " << vectorOpen[k]->player->y << "   " << vectorOpen[k]->box[0].x << "   " << vectorOpen[k]->box[0].y << endl;
        }
        for(int k = 0; k<vectorClosed.size(); k++) {
                //cout << "Element vectora closed " << k << " koordynaty  " << vectorClosed[k]->player->x << "   " << vectorClosed[k]->player->y << "   " << vectorClosed[k]->box[0].x << "   " << vectorClosed[k]->box[0].y << endl;
                //cout << "Element vectora closed " << k << " koordynaty  " << vectorClosed[k]->player->x << "   " << vectorClosed[k]->player->y << "   " << vectorClosed[k]->box[0].x << "   " << vectorClosed[k]->box[0].y << "   " << vectorClosed[k]->box[1].x << "   " << vectorClosed[k]->box[1].y << endl;
        }
        for(int k = 0; k<vectorOpen.size(); k++) {
                //cout << "Element vectora open " << k << " koordynaty  " << vectorOpen[k]->player->x << "   " << vectorOpen[k]->player->y << "   " << vectorOpen[k]->box[0].x << "   " << vectorOpen[k]->box[0].y << endl;
                //cout << "Element vectora open " << k << " koordynaty  " << vectorOpen[k]->player->x << "   " << vectorOpen[k]->player->y << "   " << vectorOpen[k]->box[0].x << "   " << vectorOpen[k]->box[0].y << "   " << vectorOpen[k]->box[1].x << "   " << vectorOpen[k]->box[1].y << endl;
        }
        vectorCheck.clear();
        }
    }
}

int main()
{
    string firstLine;
    char tab1[8];
    int ind = 0;
    int width, height, boxes;
    //ifstream fin("test1.txt", fstream::in);
    //ifstream fin("test2.txt", fstream::in);
    ifstream fin("maps/03l.txt", fstream::in);
    //ifstream fin("mapa.txt", fstream::in);
    //ifstream fin("2014competition.txt", fstream::in);
    vector<char> tempCharVector;
    vector<int> tempIntVector;
    address *box;
    address *goal;
    player_address *player_origin;

    getline(fin, firstLine);
    strncpy(tab1, firstLine.c_str(), sizeof(tab1));
    char *token = strtok(tab1," ");
    while (token != NULL) {
        if(ind==0)  { width=atoi(token); widthGlobal = width; }
        if(ind==1) { height=atoi(token); heightGlobal = height; }
        if(ind==2) { boxes=atoi(token); boxesGlobal = boxes; }
        token = strtok(NULL, " ");
        ind++;
    }

    cout << endl << "height: " << height << "   width: " << width << "   boxes: " << boxes << endl << endl;

    char tab[width][height];

    for(int i=0;i<height;i++) {
        for(int j=0;j<width;j++){
            fin >> tab[i][j];
            tempIntVector.push_back(gk);
            switch(tab[i][j]) {
            case 'J': {
                box  = new address(i,j,gk);
                vectorBoxes.push_back(*box);
                tempCharVector.push_back('J');
                break;
                }
             case 'G': {
                goal = new address(i,j,gk);
                vectorGoals.push_back(*goal);
                tempCharVector.push_back('G');
                break;
                }
            case 'M': {
                player_origin = new player_address(i,j,gk);
                tempCharVector.push_back('M');
                break;
                }
            case 'X': {
                tempCharVector.push_back('X');
                break;
            }
            case '.': {
                tempCharVector.push_back('.');
                break;
            }
            default: break;
            }
            gk++;
            cout << tab[i][j];
            }
            mapa.push_back(tempCharVector);
            mapa_pola.push_back(tempIntVector);
            tempCharVector.clear();
            tempIntVector.clear();
            cout << endl;
        }

    cout << endl << "Orignalna pozycja gracza x: " << player_origin->x << " y: " << player_origin->y << " n: " << player_origin->n << endl << endl;
    for(int i = 0; i < vectorBoxes.size(); i++) {
        cout << "Oryginalna pozycja skrzynki " << i << " x: " << vectorBoxes[i].x << " y: " << vectorBoxes[i].y << " n: " << vectorBoxes[i].n << endl << endl;
    }

    generate_children(mapa,player_origin,vectorBoxes);
    fin.close();
}
