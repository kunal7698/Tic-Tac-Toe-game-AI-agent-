#include <iostream>
#include <cassert>
#include <limits>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>
#include <map>
#include <set>
#include <deque>
#include <list>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <climits>
#include <cstring>
#include <bitset>
#include <stack>
#include <queue>
using namespace std;

#define GRID_DIM 3
char mark[2] = {'X', 'O'};

class Node{
    public:
        vector<Node*> next;
        string grid;
        int filled;
        int state;

        Node(int pstate = 2, int pfilled = 0)
        {
            next.assign(GRID_DIM*GRID_DIM, 0);
            grid.assign(GRID_DIM*GRID_DIM, '-');
            state = pstate;//will have result
            filled = pfilled;
        }
};

int winorlose(string grid)
{
    int dashes = 0;
    int cpu = 0, player = 0;
    for(int i = 0; i < GRID_DIM; i++)
    {
        cpu = 0;
        player = 0;
        for(int j = 0; j < GRID_DIM; j++)
        {
            if(grid[GRID_DIM*i+j] == 'O')
                cpu++;
            else if(grid[GRID_DIM*i+j] == 'X')
                player++;
            else
                dashes++;
        }
        if(cpu == GRID_DIM)
            return -1;
        else if(player == GRID_DIM)
            return 1;

    }

    for(int i = 0; i < GRID_DIM; i++)
    {
        cpu = 0;
        player = 0;
        for(int j = 0; j < GRID_DIM; j++)
        {
            if(grid[GRID_DIM*j+i] == 'O')
                cpu++;
            else if(grid[GRID_DIM*j+i] == 'X')
                player++;
        }
        if(cpu == GRID_DIM)
            return -1;
        else if(player == GRID_DIM)
            return 1;
    }

    cpu = 0; player = 0;
    for(int i = 0, j = 0; i < GRID_DIM; i++, j++)
    {
        if(grid[i*GRID_DIM+j] == 'O')
            cpu++;
        else if(grid[i*GRID_DIM+j] == 'X')
            player++;
    }
    if(cpu == GRID_DIM)
        return -1;
    else if(player == GRID_DIM)
        return 1;

    cpu = 0; player = 0;
    for(int i = 0, j = GRID_DIM-1; i < GRID_DIM; i++, j--)
    {
        if(grid[i*GRID_DIM+j] == 'O')
            cpu++;
        else if(grid[i*GRID_DIM+j] == 'X')
            player++;
    }
    if(cpu == GRID_DIM)
        return -1;
    else if(player == GRID_DIM)
        return 1;

    if(dashes == 0)
        return 0;
    else
        return 2;
}

int fillTree(Node& nd, int turn = 0)
{
    int result = winorlose(nd.grid);
    if(result != 2 || nd.filled == GRID_DIM*GRID_DIM)
    {
        nd.state = result;
        return result;
    }
    else
    {
        vector<int> states;
        for(int i = 0; i < GRID_DIM*GRID_DIM; i++)
        {
            if(nd.grid[i] == '-'){
                Node* tmp = new Node(nd);
                for(int i = 0; i < GRID_DIM*GRID_DIM; i++)
                    tmp->next[i] = 0;
                tmp->filled++;
                tmp->grid[i] = mark[turn];
                states.push_back(fillTree(*tmp, 1-turn));//for alternate move
                nd.next[i] = tmp;
            }
        }
        if(turn == 0)
            nd.state = *max_element(states.begin(), states.end());
        else
            nd.state = *min_element(states.begin(), states.end());
        return nd.state;
    }
}

Node* advance(Node* nd, int i = 0)
{
    return nd->next[i];
}

// cpu makes move
int cpuDecision(Node& nd)
{
    int min_index = 0, min_val = -2;
    for(int i = 0; i < GRID_DIM*GRID_DIM; i++)
    {
        //if(nd.next[i]) cout << nd.next[i]->state << " ";
        if(nd.next[i] && (nd.next[i]->state < min_val || min_val == -2))
        {
            min_val = nd.next[i]->state;
            min_index = i;
        }
    }
    cout << endl;
    return min_index;
}

void drawBoard(string grid)
{
    for(int i = 0; i < GRID_DIM; i++)
        cout << "\t" << i+1;
    cout << endl;
    for(int i = 0; i < GRID_DIM; i++)
    {
        cout << (char)(97+i);
        for(int j = 0; j < GRID_DIM; j++)
        {
            cout << "\t" << grid[i*GRID_DIM+j];
        }
        cout << endl;
    }
}

void deleteRoutine(Node* root)
{
    for(int i = 0; i < GRID_DIM*GRID_DIM; i++)
    {
        if(root->next[i]){
            deleteRoutine(root->next[i]);
            root->next[i] = 0;
        }
    }

}

int main()
{
    Node* root = new Node();
    Node* cur;
    fillTree(*root, 0);

    bool tryagain = true;
    while(tryagain){
        cur = root;
        //system("cls");
        string grid;
        grid.assign(GRID_DIM*GRID_DIM, '_');

        while(true)
        {
            drawBoard(grid);
            int row, col;
            do{
                string box;
                cin >> box;
                row = box[0]-97; col = box[1]-49;
            }
            while(row < 0 || col < 0 || row >= GRID_DIM || col >= GRID_DIM || grid[row*GRID_DIM+col]  != '_');
            grid[row*GRID_DIM+col] = 'X';

            cur = advance(cur, row*GRID_DIM+col);

            int result = winorlose(grid);
            //cout << result << endl;
            if(result == 1)
            {
                //system("cls");
                drawBoard(grid);
                cout << "Player Wins" << endl;
                break;
            }
            else if(result == 0)
            {
                //system("cls");
                drawBoard(grid);
                cout << "Draw" << endl;
                break;
            }

            int cpu = cpuDecision(*cur);
            grid[cpu] = 'O';

            cur = advance(cur, cpu);

            result = winorlose(grid);
            if(result == -1)
            {
                //system("cls");
                drawBoard(grid);
                cout << "CPU wins" << endl;
                break;
            }
            //system("cls");
        }
        cout << "Do you want to try again (y/n)?\n";
        char c;
        cin >> c;
        tryagain = c=='y';

    }
    deleteRoutine(root);
    root = 0;
    return 0;
}
