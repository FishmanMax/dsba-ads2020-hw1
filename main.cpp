#include <iostream>
#include "header.h"
#include <fstream>

void output_to_csv(const std::vector<std::vector<double>>& data, const char* fname, size_t first = 0, size_t step = 1)
{
    std::ofstream fout(fname);
    fout << "size,school,D&C,Karatsuba\n";
    for (auto results_piece : data) {
        size_t i = 0;
        fout << first << ',';
        first += step;
        for (auto result : results_piece)
            fout << result << (++i < 3 ? ',' : '\n');
    }

    fout.close();
}

int main() {
    output_to_csv(Multiplicator::test_up_to(200,0,1), "../data/output.csv");



    return 0;
}