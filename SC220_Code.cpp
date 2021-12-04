#include <bits/stdc++.h>
using namespace std;
#define debug1(x) cout<<#x<<" : "<<x<<endl;
#define debug2(x,y) cout<<#x<<" : "<<x<<"\t"<<#y<<" : "<<y<<endl;
#define debug3(x,y,z) cout<<#x<<" : "<<x<<"\t"<<#y<<" : "<<y<<"\t"<<#z<<" : "<<z<<endl;
#define fast ios_base::sync_with_stdio(false);cin.tie(NULL);
#define pb push_back
#define ll long long int
#define endl "\n"

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

//Forward declaration of functions.
int minSwapfor_A(int , string);
int minSwapfor_B(int , string);
void find_neighbouring_Node(vector<tuple<string, int, int>>& , string);

// for finding out all posible neighbour Nodes of current Node.
void find_neighbouring_Node(vector<tuple<string, int, int>> &neighbour_Node, string permutation)
{
    int n = permutation.size() , minSwap_A, minSwap_B;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            swap(permutation[i],permutation[j]);    //Swap any two elements to get neighbouring Node.

            minSwap_A = minSwapfor_A(n,permutation);
            minSwap_B = minSwapfor_B(n,permutation);
            neighbour_Node.push_back({permutation, minSwap_A, minSwap_B});
            
            swap(permutation[i],permutation[j]);        //Restore Current Permutation
        }
    }
}

bool comparator_A(const tuple<string, int, int>& t1, const tuple<string, int, int>& t2)
{
    if (get<1>(t1) == get<1>(t2))
    {
        return (get<2>(t1) > get<2>(t2));
    }
    else
    {
        return (get<1>(t1) < get<1>(t2));
    }
}

bool comparator_B(const tuple<string, int, int>& t1, const tuple<string, int, int>& t2)
{
    if (get<2>(t1) == get<2>(t2))
    {
        return (get<1>(t1) > get<1>(t2));
    }
    else
    {
        return (get<2>(t1) < get<2>(t2));
    }
}

// This Function finds minimum swaps required in current permutation for convert it in increasing order(Winning permutation for A).  
int minSwapfor_A(int n, string str)
{
    set<int> s;
    for (int i = 1; i <= n; i++) {
        s.insert(i);
    }
    int fine , cycle, start, next, nextvisit;
    fine = cycle = 0;
    
    for (int i = 0; i < n; i++) {
        if ((str[i] - '0') == i + 1) {
            fine++;
            cycle++;
            s.erase(s.find(str[i] - '0'));
        }
    }
    while (fine < n) {
        start = *s.begin();
        s.erase(s.begin());
        next = start;
        while (1) {
            next = nextvisit = str[next - 1] - '0';
            fine++;
            if (next == start) break;
            else {
                s.erase(s.find(next));
            }
        }
        cycle++;
    }
    return (fine - cycle);
}

// This Function finds minimum swaps required in current permutation for convert it in decreasing order(Winning permutation for B).  
int minSwapfor_B(int n, string str)
{
    set<int, greater<int>> s;
    for (int i = 0; i < n; i++) {
        s.insert(n - i);
    }
    int fine, cycle, start, next, nextvisit;
    fine = cycle = 0;
    
    for (int i = 0; i < n; i++) {
        if ((str[i] - '0') == n - i) {
            fine++; cycle++;
            s.erase(s.find(str[i] - '0'));
        }
    }
    while (fine < n) {
        start = *s.begin();
        s.erase(s.begin());
        next = start;
        while (1) {
            next = nextvisit = str[n - next] - '0';
            fine++;
            if (next == start) break;
            else {
                s.erase(s.find(next));
            }
        }
        cycle++;
    }
    return ( fine - cycle );
}

int main()
{
    fast

    int n;
    cin >> n;
    string permutation;
    cin >> permutation;

    map<string,int>visited_Node; 
    bool win_A = false, win_B = false;
    bool play_A = true;                         // for alternative turn.

    string increasing_order = "", decreasing_order = "";
    
    for (int i = 1;i <= n;i++)
    {
        increasing_order += (i + '0'); // 123...n (increasing order).
    }
    decreasing_order += increasing_order;
    reverse(decreasing_order.begin(),decreasing_order.end());  //n ... 321 (decreasing order).

    vector<tuple<string, int, int>> path;       // path contains (string,distance_a,distance_b).
    int minSwap_A = minSwapfor_A(n, permutation);
    int minSwap_B = minSwapfor_B(n, permutation);
    path.push_back({permutation, minSwap_A, minSwap_B});

    string current = permutation;
    visited_Node[current]++;        // mark starting string as visited.

    if (current == increasing_order) // current string is in ascending order ( = increasing_order) then A wins.
    {
        win_A = true;
    }
    else if (current == decreasing_order) // current string is in descendig order ( = decreasing_order) then B wins.
    {
        win_B = true;
    }
    else
    {
        while (!win_A && !win_B) // while current string is not in ascending or descending order then play ahead.
        {
            
            vector<tuple<string, int, int>> neighbouring_Node;

            /*finding all neighbours of current node and stored it in tuple named neighbouring_Node.*/
            find_neighbouring_Node(neighbouring_Node, current); 

            //Traverse all Neighbouring Node of current Node.
                                                                /*cout << "Before : ";
                                                                debug1(neighbouring_Node.size());*/
            for (auto itr = neighbouring_Node.begin(); itr < neighbouring_Node.end(); itr++)
            {
                //if Node is already visited than we remove it from neighbouring_Node because player can't choose that Node.
                if (visited_Node[get<0>(*itr)] != 0)
                {
                    neighbouring_Node.erase(itr);
                    itr--;
                }
            }
                                                                /*cout << "After : ";
                                                                 debug1(neighbouring_Node.size());*/
            if (play_A)         //if play_A is true than it is player A's turn.
            {
                if (neighbouring_Node.size() > 0)
                {
                    sort(neighbouring_Node.begin(), neighbouring_Node.end(), comparator_A);
                }
                else
                {
                    win_B = true;
                    break;
                }

                if (neighbouring_Node.size() > 1)
                {
                    int index = neighbouring_Node.size() - 1;
                    for (int i = 0; i < neighbouring_Node.size(); i++)
                    {
                        if (get<1>(neighbouring_Node[i]) == 0)
                        {
                            index = i;
                            break;
                        }
                        if (get<2>(neighbouring_Node[i]) != 1)
                        {
                            index = i;
                            break;
                        }
                    }
                    path.push_back({get<0>(neighbouring_Node[index]), get<1>(neighbouring_Node[index]), get<2>(neighbouring_Node[index])});
                    current = get<0>(neighbouring_Node[index]);
                }
                else
                {
                    current = get<0>(neighbouring_Node[0]);
                    path.push_back({get<0>(neighbouring_Node[0]), get<1>(neighbouring_Node[0]), get<2>(neighbouring_Node[0])});
                }
                play_A = false;
            }
            else
            {
                if (neighbouring_Node.size() > 0)
                {
                    sort(neighbouring_Node.begin(), neighbouring_Node.end(), comparator_B);
                }
                else
                {
                    win_A = true;
                    break;
                }
                if (neighbouring_Node.size() > 1)
                {
                    int index = neighbouring_Node.size() - 1;
                    for (int i = 0; i < neighbouring_Node.size(); i++)
                    {
                        if (get<2>(neighbouring_Node[i]) == 0)
                        {
                            index = i;
                            break;
                        }
                        if (get<1>(neighbouring_Node[i]) != 1)
                        {
                            index = i;
                            break;
                        }
                    }
                    current = get<0>(neighbouring_Node[index]);
                    path.push_back({get<0>(neighbouring_Node[index]), get<1>(neighbouring_Node[index]), get<2>(neighbouring_Node[index])});
                }
                else
                {
                    current = get<0>(neighbouring_Node[0]);
                    path.push_back({get<0>(neighbouring_Node[0]), get<1>(neighbouring_Node[0]), get<2>(neighbouring_Node[0])});
                }
                play_A = true;
            }

            visited_Node[current]++;

            current == increasing_order ? win_A = true : (current == decreasing_order ? win_B = true : win_B = false);

        }
    }
    
    int x;
    cout << "For Knowing Winner of a Game Enter 1 . " << endl;
    cout << "For Knowing Winner of a Game along With their Strategy (Path) Enter 2 ." << endl;
    cout << "Enter The Value : " << endl;
    cin >> x;
    switch(x)
    {
        case 1 :
        if(win_A) 
        {
            cout << "|| Winner : A ||" << endl;
        }
        else 
        {
            cout << "Winner : B ||" << endl;
        }
        break;

        case 2 : 
        if (win_A)
        {
            cout << "|| Winner : A ||" << endl << endl;
            cout << "Starting Permutation : "<<get<0>(path[0]) << "\t MinSwapfor_A : " << get<1>(path[0]) << "\t MinSwapfor_B : " << get<2>(path[0]) << " " << endl;
            cout << endl;
            for (int i = 1; i < path.size(); i++)
            {
                (i%2) ? cout << "A choose " : cout << "B choose ";
                cout <<"Permutation : " << get<0>(path[i]) << "\t MinSwapfor_A : " << get<1>(path[i]) << "\t MinSwapfor_B : " << get<2>(path[i]) << " " << endl;
                cout << endl;
            }
        }
        else
        {
            cout << "|| Winner : B ||" << endl << endl;
            cout << "Starting Permutation : "<<get<0>(path[0]) << "\t MinSwapfor_A : " << get<1>(path[0]) << "\t MinSwapfor_B : " << get<2>(path[0]) << " " << endl;
            cout << endl;
            for (int i = 1; i < path.size(); i++)
            {
                (i%2) ? cout << "A choose " : cout << "B choose ";
                cout <<"Permutation : " << get<0>(path[i]) << "\t MinSwapfor_A : " << get<1>(path[i]) << "\t MinSwapfor_B : " << get<2>(path[i]) << " " << endl;
                cout << endl;
            }
        }
    }
}
