/*
void Solve()
{
    short chooseDifficulty; cout << "Choose Test Difficulty 1 : sample , 2 : Hard .\n"; cin >> chooseDifficulty;
    string FileName, outFileName;
    if (--chooseDifficulty)
    {
        FileName += "Complete Tests/";
        cout << "\nChoose Case size: ";
        short sz; cin >> sz;
        if (sz == 1)
            FileName += "1 small/";
        else if (sz == 2)
            FileName += "2 medium/";
        else if (sz == 3)
            FileName += "3 large/";

        short inputCase; cout << "\nChoose Test Case File: ";
        cin >> inputCase;
        FileName += "Case " + to_string(inputCase) + "/Case" + to_string(inputCase);
    }
    else
    {
        short inputCase; cout << "\nChoose Test Case File: ";
        cin >> inputCase;
        FileName = "Sample Tests/Case" + to_string(inputCase);
    }
    outFileName = FileName + "-output.txt";
    FileName += ".txt";
    FileI(FileName.c_str());

    int n; cin >> n;
    vector<vector<char>> grid(n + 2, vector<char>(n + 2, '#'));

    char cell;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
        {
            while (cin >> cell, cell != '#' && cell != '.' && cell != 'o');
            grid[i][j] = cell;
        }


    pair<un short, un short> target;

    cin >> target.second, cin.ignore(), cin >> target.first;
    target.first++;
    target.second++;

    Graph g(grid, target);
    Time(Algorithm,
         auto path = g.ShortestPath_BFS();
    )

    string output, expected;
    if (path.empty())
        output += "Unsolvable";
    else
    {
        path.pop();
        output += "Solvable\nMin number of moves: " + to_string(path.size()) + "\nSequence of moves: ";
        while (!path.empty())
        {
            output += Dir[path.top()] + ", ";
            path.pop();
        }
    }
    fclose(stdin);
    FileI(outFileName.c_str());
    string tmp;
    while (getline(cin, tmp) && tmp != "Initial")
        expected += tmp + '\n';

    cout << (output += '\n');
    cerr << "\t\t\t\t\t" << (output == expected ? "ACCEPTED" : "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [REJECTED] XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
}*/


#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
using namespace std;
using namespace sf;
#ifndef ONLINE_JUDGE
#include "Debug.cpp"
#define FileI(fileName) freopen(fileName, "r", stdin);
#define FileO(fileName) freopen(fileName, "w", stdout);
#else
#define FileI(fileName)
#define FileO(fileName)
#define debug(...) 1
#define Time(i, x...) x
#endif
#define un unsigned
#define ld long double
#define LL long long
#define allc(x) begin(x), end(x)
#define rallc(x) rbegin(x), rend(x)
#define all(x,i,j) begin(x) + i, begin(x) + j
#define Test int TC, _(0); cin >> TC; while(_++ < TC)

typedef vector<pair<un short, un short>> HashedBoard;



const string Dir[] = {"left", "up", "right", "down"};

vector<vector<char>> globalBoard;

struct Graph
{
    enum Direction {NONE = -1, LEFT, UP, RIGHT, DOWN};
    struct Node
    {
        HashedBoard ballBoardState;
        Direction parentDir = NONE;
        int sourceDistance = 0;
        Node* Parent = nullptr;
        Node() = default;
        Node(const HashedBoard& ballBoardState, int sourceDistance = 0, Node* Parent = nullptr, Direction parentDir = NONE)
        {
            this->ballBoardState = ballBoardState;
            this->sourceDistance = sourceDistance;
            this->Parent = Parent;
            this->parentDir = parentDir;
        }
    };

    unsigned moveTimer = 0;
    Node* initialState;

    vector<vector<pair<un short, un short>>> obstaclesRowPos, obstaclesColPos;
    vector<pair<un short, unsigned>> ballsRowPos;
    pair<un short, un short> target;
    const short toBase = UCHAR_MAX + 1;
    vector<vector<string>> baseChar;
    bool dimentionComplexity = 1;
    vector<vector<Direction>> adj = {{UP, DOWN}, {LEFT, RIGHT}, {UP, DOWN}, {LEFT, RIGHT}};
//    vector<vector<Direction>> adj = {{DOWN, UP}, {LEFT, RIGHT}, {UP, DOWN}, {RIGHT, LEFT}};

    Graph(const vector<vector<char>>& board, const pair<un short, un short>& target)
    {
        vector<pair<un short, un short>> slidersPos;
        int n = board.size() - 2;
        globalBoard = board;
        ballsRowPos = vector<pair<un short, unsigned>>(n + 2, {-1, moveTimer});
        baseChar = vector<vector<string>>(n + 2, vector<string>(n + 2));
        for (int i = 0; i <= n + 1; i++)
            for (int j = 0; j <= n + 1; j++)
                ConvFromDec(i, baseChar[i][j]), ConvFromDec(j, baseChar[i][j]);

        obstaclesRowPos = obstaclesColPos = vector<vector<pair<un short, un short>>>(n + 2, vector<pair<un short, un short>>(n + 2));
        for (int i = 0; i <= n + 1; i++)
            for (int lj = 0, rj = n + 1, j = 0, jj = n + 1; j <= n + 1; j++, jj--)
            {
                if (board[i][j] != '#')
                    obstaclesColPos[i][j].first = lj;
                else
                    lj = j;
                if (board[i][jj] != '#')
                    obstaclesColPos[i][jj].second = rj;
                else
                    rj = jj;
                if (board[i][j] == 'o')
                    slidersPos.emplace_back(i, j), globalBoard[i][j] = '.';
            }
        for (int j = 0; j <= n + 1; j++)
            for (int ui = 0, di = n + 1, i = 0, ii = n + 1; i <= n + 1; i++, ii--)
            {
                if (board[i][j] != '#')
                    obstaclesRowPos[j][i].first = ui;
                else
                    ui = i;
                if (board[ii][j] != '#')
                    obstaclesRowPos[j][ii].second = di;
                else
                    di = ii;
            }

        initialState = new Node(slidersPos);
        this->target = target;

        auto ci = chrono::high_resolution_clock::now();
        Move(slidersPos, UP);
        auto cf = chrono::high_resolution_clock::now();
        auto squareTime = chrono::duration_cast<chrono::microseconds>(cf - ci).count();
        dimentionComplexity = 0;
        ci = chrono::high_resolution_clock::now();
        Move(slidersPos, UP);
        cf = chrono::high_resolution_clock::now();
        auto s_lg_s = chrono::duration_cast<chrono::microseconds>(cf - ci).count();
        if (squareTime <= s_lg_s)
            dimentionComplexity = 1;
        debug(squareTime);
        debug(s_lg_s);
    }

    HashedBoard Move(const HashedBoard& currentBoard, Direction dir) // O(min(s*log(s), n^2))
    {
        moveTimer++;
        unsigned sz = currentBoard.size();
        switch (dir)
        {
            case RIGHT:
            {
                auto nextBoard = currentBoard;
                un short mnj;
                for (int it = sz - 1; ~it; it--)
                {
                    auto& [x, y] = nextBoard[it];
                    mnj = obstaclesColPos[x][y].second;
                    if (it != sz - 1 && nextBoard[it + 1].first == x)
                        mnj = min(mnj, nextBoard[it + 1].second);
                    y = mnj - 1;
                }
                return nextBoard;
            }
            case LEFT:
            {
                auto nextBoard = currentBoard;
                un short mxj;
                for (int it = 0; it < sz; it++)
                {
                    auto& [x, y] = nextBoard[it];
                    mxj = obstaclesColPos[x][y].first;
                    if (it && nextBoard[it - 1].first == x)
                        mxj = max(mxj, nextBoard[it - 1].second);
                    y = mxj + 1;
                }
                return nextBoard;
            }
            case UP:
            {
                if (dimentionComplexity)
                {
                    unsigned n = globalBoard.size() - 2;
                    HashedBoard nextBoard;
                    un short mxi;
                    for (int it = 0; it < sz; it++)
                    {
                        auto& [x, y] = currentBoard[it];
                        mxi = obstaclesRowPos[y][x].first;
                        if (ballsRowPos[y].second < moveTimer)
                            ballsRowPos[y].second = moveTimer;
                        else
                            mxi = max(mxi, ballsRowPos[y].first);
                        ballsRowPos[y].first = mxi + 1;
                        globalBoard[mxi + 1][y] = 'o';
                    }
                    for (int i = 1; i <= n; i++)
                        for (int j = 1; j <= n; j++)
                            if (globalBoard[i][j] == 'o')
                                globalBoard[i][j] = '.', nextBoard.emplace_back(i, j);
                    return nextBoard;
                }
                else
                {
                    HashedBoard nextBoard;
                    un short mxi;
                    for (int it = 0; it < sz; it++)
                    {
                        auto& [x, y] = currentBoard[it];
                        mxi = obstaclesRowPos[y][x].first;
                        if (ballsRowPos[y].second < moveTimer)
                            ballsRowPos[y].second = moveTimer;
                        else
                            mxi = max(mxi, ballsRowPos[y].first);
                        ballsRowPos[y].first = mxi + 1;
                        nextBoard.emplace_back(mxi + 1, y);
                    }
                    stable_sort(allc(nextBoard));
                    return nextBoard;
                }
            }
            case DOWN:
            {
                if (dimentionComplexity)
                {
                    unsigned n = globalBoard.size() - 2;
                    HashedBoard nextBoard;
                    un short mni;
                    for (int it = sz - 1; ~it; it--)
                    {
                        auto& [x, y] = currentBoard[it];
                        mni = obstaclesRowPos[y][x].second;
                        if (ballsRowPos[y].second < moveTimer)
                            ballsRowPos[y].second = moveTimer;
                        else
                            mni = min(mni, ballsRowPos[y].first);
                        ballsRowPos[y].first = mni - 1;
                        globalBoard[mni - 1][y] = 'o';
                    }
                    for (int i = 1; i <= n; i++)
                        for (int j = 1; j <= n; j++)
                            if (globalBoard[i][j] == 'o')
                                globalBoard[i][j] = '.', nextBoard.emplace_back(i, j);

                    return nextBoard;
                }
                else
                {
                    HashedBoard nextBoard;
                    un short mni;
                    for (int it = sz - 1; ~it; it--)
                    {
                        auto& [x, y] = currentBoard[it];
                        mni = obstaclesRowPos[y][x].second;
                        if (ballsRowPos[y].second < moveTimer)
                            ballsRowPos[y].second = moveTimer;
                        else
                            mni = min(mni, ballsRowPos[y].first);
                        ballsRowPos[y].first = mni - 1;
                        nextBoard.emplace_back(mni - 1, y);
                    }
                    stable_sort(allc(nextBoard));
                    return nextBoard;
                }
            }
            case NONE:
                return currentBoard;
        }
        return currentBoard;
    }

    vector<vector<char>> functionsare3a(HashedBoard c)
    {
        vector<vector<char>> tmp = globalBoard;
        for (auto& [x, y] : c)
            tmp[x][y] = 'o';
        return tmp;
    }

    inline void ConvFromDec(un short num, string& ret)
    {
        while (num)
            ret += (num % toBase), num /= toBase;
        ret += ' ';
    }

    string ContainerHashing(const HashedBoard& container)
    {
        string ret;
        unsigned sz = container.size();
        for (int it = 0; it < sz; it++)
            ret += baseChar[container[it].first][container[it].second];
        ret.pop_back();
        return ret;
    }

    Node* ShortestPath_BFS()
    {
        // delete new-operator-defined nodes
        queue<Node*> nxt;
        unordered_set<string> vis;
        vis.insert(ContainerHashing(initialState->ballBoardState));
        if (binary_search(allc(initialState->ballBoardState), target))
        {
            return initialState;
        }

        for (auto dir : {LEFT, UP, RIGHT, DOWN})
        {
            auto newBoardState = move(Move(initialState->ballBoardState, dir));
            if (vis.insert(ContainerHashing(newBoardState)).second)
            {
                auto child = new Node(newBoardState, initialState->sourceDistance + 1, initialState, dir);
                if (binary_search(allc(newBoardState), target))
                {
                    
                    return child;
                }
                nxt.push(child);
            }
        }

        while (!nxt.empty())
        {
            auto cur = nxt.front(); nxt.pop();
            //if (!(rand() % 10000))
            //debug(cur->sourceDistance);

            for (auto dir : adj[cur->parentDir])
            {
                //Time(BFS,
                //debug(Dir[dir]);
                //Time(MOVE,
                auto newBoardState = move(Move(cur->ballBoardState, dir));
                //)
                //Time(Hashing,
                const string& str = ContainerHashing(newBoardState);
                //) // 60microsec
                //Time(INSERT,
                bool ok = vis.insert(str).second;
                //)
                if (ok)
                {
//                    debug(functionsare3a(cur->ballBoardState)),
//                    cerr << "--->\n",
//                    debug(functionsare3a(newBoardState)),
//                    cerr << "\n\n\n",
                    //Time(PUSH,
                    auto child = new Node(newBoardState, cur->sourceDistance + 1, cur, dir);
                    //)
                    if (binary_search(allc(newBoardState), target))
                    {
                        
                        return child;
                    }

                    nxt.push(child);
                }
                //)
            }
        }
        return NULL;
    }
};

vector<vector<char>> functionsare3a(HashedBoard c)
{
    vector<vector<char>> tmp = globalBoard;
    for (auto& [x, y] : c)
        tmp[x][y] = 'o';
    return tmp;
}

sf::RenderWindow window(sf::VideoMode(1200, 800), "SFML GUI");

void GUI() {



    Texture backgroundTexture;
    backgroundTexture.loadFromFile("white.jpg");
    Sprite background;
    background.setTexture(backgroundTexture);

    Texture buttonTexture;
    buttonTexture.loadFromFile("button.png");
    Sprite next;
    next.setTexture(buttonTexture);
    next.setScale(0.18, 0.18);
    next.setPosition(65, 500);

    Sprite previous;
    previous.setTexture(buttonTexture);
    previous.setScale(0.18, 0.18);
    previous.setPosition(65, 400);

    Sprite solve;
    solve.setTexture(buttonTexture);
    solve.setScale(0.18, 0.18);
    solve.setPosition(65, 300);

    Text nextText, prevText, solveText, tilt, game, solveStatus;
    Font font;
    font.loadFromFile("arial.ttf");
    tilt.setColor(Color::Black);
    tilt.setFont(font);
    tilt.setString("Tilt");
    tilt.setCharacterSize(70);
    tilt.setPosition(120, 100);
    tilt.setStyle(sf::Text::Bold);

    game.setColor(Color::Black);
    game.setFont(font);
    game.setString("Game");
    game.setCharacterSize(70);
    game.setPosition(100, 170);
    game.setStyle(sf::Text::Bold);

    nextText.setFont(font);
    nextText.setString("Next");
    nextText.setCharacterSize(50);
    nextText.setPosition(150, 500);
    nextText.setStyle(sf::Text::Bold);

    //user.setColor(Color::Green);
    prevText.setFont(font);
    prevText.setString("Previous");
    prevText.setCharacterSize(50);
    prevText.setPosition(100, 400);
    prevText.setStyle(sf::Text::Bold);

    solveText.setFont(font);
    solveText.setString("Solve");
    solveText.setCharacterSize(50);
    solveText.setPosition(140, 300);
    solveText.setStyle(sf::Text::Bold);


    solveStatus.setFont(font);

    solveStatus.setCharacterSize(50);
    solveStatus.setPosition(120, 600);
    solveStatus.setStyle(sf::Text::Bold);

    string FileName = "input.txt";
    FileI(FileName.c_str());

    int n; cin >> n;
    vector<vector<char>> grid(n + 2, vector<char>(n + 2, '#'));

    pair<un short, un short> t;

    stack<Graph::Node*> nextNode;
    Graph::Node* path;

    Graph g(vector<vector<char>> x = {}, pair<un short, un short>p = {0, 0});
    bool checkSolve = false;
    while(window.isOpen()) {

        Event event;
        while(window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                // Get the mouse position
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Check if the mouse position is within the button's bounds
                if (solve.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    std::cout << "Button Clicked!" << std::endl;


                    char cell;
                    for (int i = 1; i <= n; i++)
                        for (int j = 1; j <= n; j++)
                        {
                            while (cin >> cell, cell != '#' && cell != '.' && cell != 'o');
                            grid[i][j] = cell;
                        }


                    pair<un short, un short> target;

                    cin >> target.second, cin.ignore(), cin >> target.first;
                    target.first++;
                    target.second++;
                    t = target;
                    Graph g(grid, target);
                    solveStatus.setString("Solving...");
                    solveStatus.setColor(Color::Black);
                    window.draw(solveStatus);
                    window.display();
                    path = g.ShortestPath_BFS();
                    if(path != NULL) {
                        checkSolve = true;
                        solveStatus.setString("Solved");
                        solveStatus.setColor(Color::Green);
                        cout << path->ballBoardState[0].first;
                        grid = g.functionsare3a(path->ballBoardState);
                        cout << grid[0][0];
                    } else {
                        solveStatus.setString("Unsolvable");
                        solveStatus.setColor(Color::Red);
                        solveStatus.setPosition(80, 600);
                        cout << "unsolvable";
                    }


                }
                else if (next.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    if(!nextNode.empty() && checkSolve) {
                        path = nextNode.top();
                        nextNode.pop();
                        grid = functionsare3a(path->ballBoardState);
                    }

                }
                else if (previous.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    if(checkSolve && path->Parent) {
                        nextNode.push(path);
                        path = path->Parent;
                        grid = functionsare3a(path->ballBoardState);
                    }

                }

            }
            //cout << grid.empty();
        }
        //cout << grid.empty();
        const int windowWidth = 1200;
        const int windowHeight = 600;
        int cellSizeX;
        int cellSizeY;
        int cellMargin;
        if (n<600) {
            cellMargin = 1;
            cellSizeX = ((600) - cellMargin) / n;
            cellSizeY = ((600) - cellMargin) / n;
        }
        else
        {
            cellMargin=0.1;
            cellSizeX = 1;
            cellSizeY = 1;
        }






        Texture ball;
        ball.loadFromFile("bb.png");
        window.clear();
        window.draw(background);
        window.draw(next);
        window.draw(previous);
        window.draw(nextText);
        window.draw(prevText);
        window.draw(solve);
        window.draw(solveText);
        window.draw(tilt);
        window.draw(game);
        window.draw(solveStatus);

        if(!grid.empty()){

            sf::RectangleShape container;
            container.setSize(sf::Vector2f(grid[0].size() * (cellSizeX + cellMargin) - cellMargin, grid.size() * (cellSizeY + cellMargin) - cellMargin));
            container.setFillColor(sf::Color::Transparent);
            container.setOutlineColor(sf::Color::Red);
            container.setOutlineThickness(2.0f);
            container.setPosition(400, 80);

            for (size_t y = 0; y < grid.size(); ++y)
            {
                for (size_t x = 0; x < grid[y].size(); ++x)
                {
                    sf::RectangleShape cell(sf::Vector2f(cellSizeX, cellSizeY));
                    cell.setPosition(container.getPosition().x + x * (cellSizeX + cellMargin), container.getPosition().y + y * (cellSizeY + cellMargin));
                    if(y == t.first && x == t.second)
                        cell.setFillColor(sf::Color::Red);
                    if (grid[y][x] == '#')
                        cell.setFillColor(sf::Color::Black);
                    else if (grid[y][x] == 'o')
                        cell.setTexture(&ball);
                    else if(grid[y][x] == ' ')
                        cell.setFillColor(sf::Color::White);
                    window.draw(cell);
                }
            }
        }





        window.display();
    }
}


int main() {

    GUI();
    return 0;
}
