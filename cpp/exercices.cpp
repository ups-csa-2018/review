#include <cstdlib>
#include <iostream>
#include <array>
#include <functional>
#include <utility>
#include <cmath>
#include <algorithm>

void exercise1()
{
    int age = 10;
    std::cout << "You are " << age << " years old." << std::endl;
}

void exercise2()
{
    // Yeah there is no "native" string type on C++ but the Standard library provides one,
    // the std::string type.
    std::string name;
    std::cout << "What's your name?" << std::endl;
    std::getline(std::cin, name);

    if (std::cin.fail()) {
        std::cerr << "Unable to read your name." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "OK, your name is: " << name << std::endl;
}

void exercise3()
{
    int x;
    int y;

    std::cout << "Give me x: " << std::endl;
    std::cin >> x;

    if (std::cin.fail()) {
        std::cerr << "Unable to read x." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Give me y: " << std::endl;
    std::cin >> y;

    if (std::cin.fail()) {
        std::cerr << "Unable to read y." << std::endl;
        exit(EXIT_FAILURE);
    }

    int p = x * y;
    int s = x + y;

    std::cout << "Total: " << 2 * s + p * (s - y) * (p + x) << std::endl;
}

void exercise4()
{
    std::array<std::pair<std::string, std::function<int(int, int)>>, 5> choices = {
        std::make_pair("Add", std::plus<int>()),
        std::make_pair("Substract", std::minus<int>()),
        std::make_pair("Multiply", std::multiplies<int>()),
        std::make_pair("Divide", std::divides<int>()),
        std::make_pair("Modulus", std::modulus<int>())
    };

    std::cout << "MENU:" << std::endl;

    for (auto it = choices.begin(); it != choices.end(); it++) {
        std::cout << "(" << (std::distance(choices.begin(), it) + 1) << ") "
            << it->first << std::endl;
    }

    std::size_t user_choice;

    std::cout << "Enter your choice:" << std::endl;
    std::cin >> user_choice;

    if (std::cin.fail()) {
        std::cerr << "Unable to read your choice." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (user_choice < 1 || user_choice > choices.size()) {
        std::cerr << "Your choice must be between 1 and "
            << choices.size() << "." << std::endl;
        exit(EXIT_FAILURE);
    }

    auto choice = choices[user_choice - 1];

    int x;
    int y;

    std::cout << "Enter two numbers:" << std::endl;
    std::cin >> x >> y;

    if (std::cin.fail()) {
        std::cerr << "Unable to read your numbers." << std::endl;
        exit(EXIT_FAILURE);
    }

    if ((choice.first == "Divide" || choice.first == "Modulus") && y == 0) {
        std::cerr << "Looks like you want a division by zero..." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Result of division will be casted to int, but who cares?
    std::cout << "Result: " << choice.second(x, y) << std::endl;
}

void exercise5(int n = 0, bool increase = true)
{
    std::cout << n << " ";

    if (!increase && n == 0) {
        std::cout << std::endl;
        return;
    }

    if (n >= 100) {
        exercise5(n - 1, false);
    } else if (increase) {
        exercise5(n + 1, true);
    } else {
        exercise5(n - 1, false);
    }
}

void exercise6()
{
    const short blank = 9;
    const short items = 9;

    std::array<int, items> numbers = {1, 2, 3, 4, 5, 6, 7, 8, blank};
    std::random_shuffle(numbers.begin(), numbers.end());

    while (!std::is_sorted(numbers.begin(), numbers.end())) {
        for (int i = 0; i < items; i++) {
            if (numbers[i] == blank) {
                std::cout << " ";
            } else {
                std::cout << numbers[i];
            }
            std::cout << " ";

            if ((i + 1) % 3 == 0) {
                std::cout << std::endl;
            }
        }

        int move = 0;

        std::cout << "Type the number to move:" << std::endl;
        std::cin >> move;

        if (std::cin.fail()) {
            std::cout << "Unable to read your number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (move < 1 || move >= items) {
            std::cout << "Choose a number on your screen..." << std::endl;
            continue;
        }

        auto move_iter = std::find(numbers.begin(), numbers.end(), move);
        auto blank_iter = std::find(numbers.begin(), numbers.end(), blank);
        int dist = std::distance(move_iter, blank_iter);
        int pos_blank = std::distance(numbers.begin(), blank_iter);

        if (std::abs(dist) != 3
            && (dist != -1 || (pos_blank + 1) % 3 == 0)
            && (dist != 1 || (pos_blank) % 3 == 0)) {
            std::cout << "You can't move this number..." << std::endl;
            continue;
        }

        std::iter_swap(move_iter, blank_iter);
    }

    std::cout << "You win! Congrats!" << std::endl;
}

namespace exercise7
{
    class Rectangle
    {
        private:
            const int h;
            const int w;

        public:
            Rectangle(int h, int w): h(h), w(w) {};

            int getPerimeter() const { return 2 * (h + w); };

            int getSurface() const { return h * w; };

            void display(std::ostream &stream) const
            {
                for (int i = 0; i < h; i++) {
                    for (int j = 0; j < w; j++) {
                        stream << "*";
                    }
                    stream << std::endl;
                }
            }
    };

    void main()
    {
        Rectangle rect(2, 6);

        rect.display(std::cout);
    }
}

namespace exercise8
{
    class Counter
    {
        private:
            unsigned int n;

        public:
            Counter(unsigned int n = 0): n(n) {};

            void increment() { n++; };
            void decrement()
            {
                if (n == 0) {
                    // maybe throw an exception or smthg like that?
                    return;
                }

                n--;
            };

            unsigned int get() const { return n; };
    };

    void main()
    {
        Counter counter(5);

        std::cout << counter.get() << std::endl;
        for (int i = 0; i < 10; i++) {
            counter.increment();
        }

        std::cout << counter.get() << std::endl;
        for (int i = 0; i < 10; i++) {
            counter.decrement();
        }

        std::cout << counter.get() << std::endl;
    }
}

namespace exercise9
{
    class Point
    {
        private:
            int x;
            int y;

        public:
            Point(int x, int y): x(x), y(y) {};

            void display(std::ostream &stream) const
            {
                stream << "(" << x << "," << y << ")" << std::endl;
            }

            friend bool compare(const Point &p1, const Point &p2);
            friend int distance(const Point &p1, const Point &p2);
            friend Point middle(const Point &p1, const Point &p2);
    };

    bool compare(const Point &p1, const Point &p2)
    {
        return p1.x == p2.x && p1.y == p2.y;
    }

    int distance(const Point &p1, const Point &p2)
    {
        return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
    }

    Point middle(const Point &p1, const Point &p2)
    {
        return Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
    }

    void main()
    {
        Point p1(2, 2);
        Point p2(4, 4);
        p1.display(std::cout);
        p2.display(std::cout);

        std::cout << "compare: " << compare(p1, p2) << std::endl;
        std::cout << "distance: " << distance(p1, p2) << std::endl;
        std::cout << "middle: ";
        middle(p1, p2).display(std::cout);
    }
}

void init()
{
    unsigned int seed = time(NULL);
    srand(seed);
}

int main(int argc, char **argv)
{
    int exercise_number;

    if (argc != 2) {
        std::cerr << "Usage: exercices <exercise-number>" << std::endl;
        exit(EXIT_FAILURE);
    }

    exercise_number = atoi(argv[1]);

    init();

    switch (exercise_number) {
        case 1: exercise1(); break;
        case 2: exercise2(); break;
        case 3: exercise3(); break;
        case 4: exercise4(); break;
        case 5: exercise5(); break;
        case 6: exercise6(); break;
        case 7: exercise7::main(); break;
        case 8: exercise8::main(); break;
        case 9: exercise9::main(); break;
        default:
            std::cerr << "exercise " << exercise_number
                << " does not exist or is not implemented yet." << std::endl;
            exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
