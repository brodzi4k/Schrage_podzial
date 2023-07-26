/* 
Autorzy:
Kacper Brodziak
Krzysztof Mak
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>

struct Zadanie
{
    int id; /* Identyfikator zadania */
    int termin; /* Czas dostępności */
    int czasWykonania; /* Czas wykonania */
    int czasDostarczenia; /* Czas dostarczenia */

    bool operator==(const Zadanie& other) const {
        return id == other.id && termin == other.termin && czasWykonania == other.czasWykonania && czasDostarczenia == other.czasDostarczenia;
    }
};

int schrage(const std::vector<Zadanie>& zadania, std::vector<int>& permutacja)
{
    std::vector<Zadanie> N = zadania; // Kopiowanie listy zadań
    std::vector<Zadanie> G; // Zbiór zadań gotowych do realizacji
    int t = 0; // Chwila czasowa
    int Cmax = 0; // Wartość Cmax
    Zadanie l = {0, 0, 0, std::numeric_limits<int>::max()}; // Aktualnie wykonywane zadanie

    while (!G.empty() || !N.empty())
    {
        while (!N.empty() && std::min_element(N.begin(), N.end(), [](const Zadanie& a, const Zadanie& b) {
            return a.termin < b.termin;
        })->termin <= t)
        {
            auto e = *std::min_element(N.begin(), N.end(), [](const Zadanie& a, const Zadanie& b) {
                return a.termin < b.termin;
            });
            G.push_back(e);
            N.erase(std::find(N.begin(), N.end(), e));

            if (e.czasDostarczenia > l.czasDostarczenia)
            {
                int pl = t - e.termin;
                t = e.termin;
                if (pl > 0)
                {
                    G.push_back({l.id, l.termin, pl, l.czasDostarczenia});
                }
            }
        }

        if (G.empty())
        {
            t = std::min_element(N.begin(), N.end(), [](const Zadanie& a, const Zadanie& b) {
                return a.termin < b.termin;
            })->termin;
        }
        else
        {
            auto e = *std::max_element(G.begin(), G.end(), [](const Zadanie& a, const Zadanie& b) {
                return a.czasDostarczenia < b.czasDostarczenia;
            });
            G.erase(std::find(G.begin(), G.end(), e));
            l = e;
            t += e.czasWykonania;
            Cmax = std::max(Cmax, t + e.czasDostarczenia);
            permutacja.push_back(e.id);
        }
    }

    return Cmax;
}

int main()
{
    std::vector<Zadanie> zadania;
    std::vector<int> permutacja;
    int N, R[100], P[100], Q[100];
    int data_id;

    /* Dane do wczytania pliku */
    std::ifstream file("data.txt");
    std::string z = "data.00";
    std::string x = "schrpmtn:";
    std::string s;
    std::ostringstream data;

    /* Wybór partii danych z pliku */
    std::cout << "Podaj numer partii danych (0-8): " << std::endl;
    std::cin >> data_id;
    if (data_id > 8 || data_id < 0)
    {
        std::cerr << "Nieprawidłowy numer partii danych" << std::endl;
        return 1;
    }
    else
    {
        z += std::to_string(data_id) + ":";
    }

    while (s != z)
    {
        file >> s;
    }

    file >> N;

    for (int i = 0; i < N; i++)
    {
        file >> R[i] >> P[i] >> Q[i];
        zadania.push_back({i + 1, R[i], P[i], Q[i]}); // Przypisanie unikalnego id
    }
    file.close();

    int cmax = schrage(zadania, permutacja);

    /* Wyświetlenie permutacji */
    std::cout << "Permutacja: ";
    for (int i : permutacja)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    /* Wyświetlenie Cmax */
    std::cout << "Cmax: " << cmax << std::endl;

    return 0;
}
