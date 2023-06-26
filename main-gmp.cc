#include "src/lib-gmp.h"

#include <iostream>

int main()
{

	const int rows = 2;
	const int cols = 2;

	mpq_t row_payoff_data[rows * cols];
	mpq_t col_payoff_data[rows * cols];

	for (int i = 0; i < rows * cols; ++i) {
		mpq_init(row_payoff_data[i]);
		mpq_init(col_payoff_data[i]);
	}

	mpq_set_ui(row_payoff_data[0], 2, 2);
	mpq_set_ui(row_payoff_data[1], 0, 1);
	mpq_set_ui(row_payoff_data[2], 0, 1);
	mpq_set_ui(row_payoff_data[3], 1, 1);

	mpq_set_ui(col_payoff_data[0], 0, 2);
	mpq_set_ui(col_payoff_data[1], 1, 1);
	mpq_set_ui(col_payoff_data[2], 1, 1);
	mpq_set_ui(col_payoff_data[3], 0, 1);

	game Game;

	lrs_mp_vector row_data = alloc_data(rows + 2);
	lrs_mp_vector col_data = alloc_data(cols + 2);

	solve_2(&Game, rows, cols, row_payoff_data, col_payoff_data, row_data, col_data);

    std::cout << "row_strategy: " << std::endl;
    for (int row_idx = 0; row_idx < rows + 2; ++row_idx) {
        std::cout <<  mpz_get_ui(row_data[row_idx]) << ' ';
    }
    std::cout << std::endl;

    std::cout << "col_strategy: " << std::endl;
    for (int col_idx = 0; col_idx < cols + 2; ++col_idx) {
        std::cout << mpz_get_ui(col_data[col_idx]) << ' ';
    }
    std::cout << std::endl;

	dealloc_data(row_data, rows + 2);
	dealloc_data(col_data, cols + 2);

	return 0;
}
