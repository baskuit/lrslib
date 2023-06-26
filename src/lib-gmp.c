#include "lib-gmp.h"
#include "float.h"

lrs_mp_vector alloc_data(size_t size)
{
    return lrs_alloc_mp_vector(size);
}

void dealloc_data(lrs_mp_vector data, size_t size)
{
    lrs_clear_mp_vector(data, size);
}

void init_game(game *g, int rows, int cols, int *row_num, int *row_den, int *col_num, int *col_den)
{
    lrs_init("*lrsnash:");
    g->nstrats[0] = rows;
    g->nstrats[1] = cols;
    int flat_idx = 0;
    for (int row_idx = 0; row_idx < rows; ++row_idx)
    {
        for (int col_idx = 0; col_idx < cols; ++col_idx)
        {
            g->payoff[row_idx][col_idx][0].num = row_num[flat_idx];
            g->payoff[row_idx][col_idx][0].den = row_den[flat_idx];
            g->payoff[row_idx][col_idx][1].num = col_num[flat_idx];
            g->payoff[row_idx][col_idx][1].den = col_den[flat_idx];
            ++flat_idx;
        }
    }
}

void solve(game *g, lrs_mp_vector row_data, lrs_mp_vector col_data)
{
    lrs_solve_nash_(g, row_data, col_data);
}

int lrs_solve_nash_(game *g, lrs_mp_vector row_data, lrs_mp_vector col_data)
{
    lrs_dic *P1;      /* structure for holding current dictionary and indices */
    lrs_dat *Q1, *Q2; /* structure for holding problem data            */

    lrs_mp_vector output1; /* holds one line of output; ray,vertex,facet,linearity */
    lrs_mp_vector output2; /* holds one line of output; ray,vertex,facet,linearity */
    lrs_mp_matrix Lin;     /* holds input linearities if any are found             */
    lrs_mp_matrix A2orig;
    lrs_dic *P2orig; /* we will save player 2's dictionary in getabasis      */

    long *linindex; /* for faster restart of player 2                       */

    long col; /* output column index for dictionary                   */
    long startcol = 0;
    long prune = FALSE;  /* if TRUE, getnextbasis will prune tree and backtrack  */
    long numequilib = 0; /* number of nash equilibria found                      */
    long oldnum = 0;

    Q1 = lrs_alloc_dat("LRS globals"); /* allocate and init structure for static problem data */
    if (Q1 == NULL)
    {
        return 0;
    }

    Q1->nash = TRUE;
    Q1->n = g->nstrats[ROW] + 2;
    Q1->m = g->nstrats[ROW] + g->nstrats[COL] + 1;

    Q1->debug = Debug_flag;
    Q1->verbose = Verbose_flag;

    P1 = lrs_alloc_dic(Q1); /* allocate and initialize lrs_dic */
    if (P1 == NULL)
    {
        return 0;
    }

    BuildRep(P1, Q1, g, 1, 0);

    output1 = lrs_alloc_mp_vector(Q1->n + Q1->m); /* output holds one line of output from dictionary     */

    /* allocate and init structure for player 2's problem data */
    Q2 = lrs_alloc_dat("LRS globals");
    if (Q2 == NULL)
    {
        return 0;
    }

    Q2->debug = Debug_flag;
    Q2->verbose = Verbose_flag;

    Q2->nash = TRUE;
    Q2->n = g->nstrats[COL] + 2;
    Q2->m = g->nstrats[ROW] + g->nstrats[COL] + 1;

    P2orig = lrs_alloc_dic(Q2); /* allocate and initialize lrs_dic */
    if (P2orig == NULL)
    {
        return 0;
    }
    BuildRep(P2orig, Q2, g, 0, 1);
    A2orig = P2orig->A;

    output2 = lrs_alloc_mp_vector(Q1->n + Q1->m); /* output holds one line of output from dictionary     */

    linindex = calloc((P2orig->m + P2orig->d + 2), sizeof(long)); /* for next time */

    if (!lrs_getfirstbasis(&P1, Q1, &Lin, TRUE))
        return 1;

    if (Q1->homogeneous && Q1->hull)
        startcol++;
    col = startcol;

    do
    {
        prune = lrs_checkbound(P1, Q1);
        if (!prune && lrs_getsolution(P1, Q1, output1, col))
        {
            oldnum = numequilib;

            lrs_dic *P2;       /* This can get resized, cached etc. Loaded from P2orig */
            lrs_mp_matrix Lin; /* holds input linearities if any are found             */
            long col;          /* output column index for dictionary                   */
            long startcol = 0;
            long prune = FALSE; /* if TRUE, getnextbasis will prune tree and backtrack  */
            long nlinearity;
            long *linearity;

            long i, j;

            P2 = lrs_getdic(Q2);
            copy_dict(Q2, P2, P2orig);

            linearity = Q2->linearity;
            nlinearity = 0;
            for (i = Q1->lastdv + 1; i <= P1->m; i++)
            {
                if (!__zero(P1->A[P1->Row[i]][0]))
                {
                    j = Q1->inequality[P1->B[i] - Q1->lastdv];
                    if (Q1->nlinearity == 0 || j < Q1->linearity[0])
                        linearity[nlinearity++] = j;
                }
            }
            if (Q1->nlinearity > 0)
                linearity[nlinearity++] = Q1->linearity[0];

            for (i = 1; i < nlinearity; i++)
                reorder(linearity, nlinearity);

            Q2->nlinearity = nlinearity;
            Q2->polytope = FALSE;

            if (!lrs_getfirstbasis2(&P2, Q2, P2orig, &Lin, TRUE, linindex))
                goto sayonara;

            if (Q2->homogeneous && Q2->hull)
                startcol++;

            do
            {
                prune = lrs_checkbound(P2, Q2);
                col = 0;
                if (!prune && lrs_getsolution(P2, Q2, output2, col))
                {
                    long lrs_nashoutput1 = TRUE;
                    long i1;
                    long origin = TRUE;

                    for (i1 = 1; i1 < Q2->n; i1++)
                        if (!__zero(output2[i1]))
                            origin = FALSE;

                    if (origin)
                        lrs_nashoutput1 = FALSE;

                    if (lrs_nashoutput1)
                    {
                        for (i1 = 1; i1 < Q2->n; i1++)
                            prat_("", output2[i1], output2[0]);

                        for (i1 = 0; i1 < Q2->n; i1++)
                        {
                            __copy(col_data[i1], output2[i1]);
                        }

                        numequilib++;
                    }
                }
            } while (lrs_getnextbasis(&P2, Q2, prune));

        sayonara:
            lrs_free_dic(P2, Q2);

            if (numequilib > oldnum || Q1->verbose)
            {
                long lrs_nashoutput2 = TRUE;
                long i1;
                long origin = TRUE;

                for (i1 = 1; i1 < Q1->n; i1++)
                    if (!__zero(output1[i1]))
                        origin = FALSE;

                if (origin)
                    lrs_nashoutput2 = FALSE;

                if (lrs_nashoutput2)
                {
                    for (i1 = 1; i1 < Q1->n; i1++)
                        prat_("", output1[i1], output1[0]);

                    for (i1 = 0; i1 < Q1->n; i1++)
                    {
                        __copy(row_data[i1], output1[i1]);
                    }
                    goto done;
                }
            }
        }
    } while (lrs_getnextbasis(&P1, Q1, prune));

done:
    lrs_clear_mp_vector(output1, Q1->m + Q1->n);
    lrs_clear_mp_vector(output2, Q1->m + Q1->n);

    lrs_free_dic(P1, Q1); /* deallocate lrs_dic */
    lrs_free_dat(Q1);     /* deallocate lrs_dat */

    Q2->Qhead = P2orig; /* reset this or you crash free_dic */
    P2orig->A = A2orig; /* reset this or you crash free_dic */

    lrs_free_dic(P2orig, Q2); /* deallocate lrs_dic */
    lrs_free_dat(Q2);         /* deallocate lrs_dat */

    free(linindex);
    return 0;
}

// New solve that skips the game stuff

void lrs_set_row_mp_(lrs_dic *P, lrs_dat *Q, long row, lrs_mp_vector num, lrs_mp_vector den, long ineq)
/* set row of dictionary using num and den arrays for rational input */
/* ineq = 1 (GE)   - ordinary row  */
/*      = 0 (EQ)   - linearity     */
{
    lrs_mp Temp, mpone;
    lrs_mp_vector oD; /* denominator for row  */

    long i, j;

    /* assign local variables to structures */

    lrs_mp_matrix A;
    lrs_mp_vector Gcd, Lcm;
    long hull;
    long m, d;
    lrs_alloc_mp(Temp);
    lrs_alloc_mp(mpone);
    hull = Q->hull;
    A = P->A;
    m = P->m;
    d = P->d;
    Gcd = Q->Gcd;
    Lcm = Q->Lcm;

    oD = lrs_alloc_mp_vector(d);
    itomp(ONE, mpone);
    itomp(ONE, oD[0]);

    i = row;
    itomp(ONE, Lcm[i]);         /* Lcm of denominators */
    itomp(ZERO, Gcd[i]);        /* Gcd of numerators */
    for (j = hull; j <= d; j++) /* hull data copied to cols 1..d */
    {
        __copy(A[i][j], num[j - hull]);
        __copy(oD[j], den[j - hull]);
        if (!one(oD[j]))
            lcm(Lcm[i], oD[j]); /* update lcm of denominators */
        __copy(Temp, A[i][j]);
        gcd(Gcd[i], Temp); /* update gcd of numerators   */
    }

    if (hull)
    {
        itomp(ZERO, A[i][0]);             /*for hull, we have to append an extra column of zeroes */
        if (!one(A[i][1]) || !one(oD[1])) /* all rows must have a one in column one */
            Q->polytope = FALSE;
    }
    if (!__zero(A[i][hull]))    /* for H-rep, are zero in column 0     */
        Q->homogeneous = FALSE; /* for V-rep, all zero in column 1     */

    storesign(Gcd[i], POS);
    storesign(Lcm[i], POS);
    if (mp_greater(Gcd[i], mpone) || mp_greater(Lcm[i], mpone))
        for (j = 0; j <= d; j++)
        {
            exactdivint(A[i][j], Gcd[i], Temp); /*reduce numerators by Gcd  */
            mulint(Lcm[i], Temp, Temp);         /*remove denominators */
            exactdivint(Temp, oD[j], A[i][j]);  /*reduce by former denominator */
        }

    if (ineq == EQ) /* input is linearity */
    {
        Q->linearity[Q->nlinearity] = row;
        Q->nlinearity++;
    }

    /* 2010.4.26   Set Gcd and Lcm for the non-existant rows when nonnegative set */

    if (Q->nonnegative && row == m)
        for (j = 1; j <= d; j++)
        {
            itomp(ONE, Lcm[m + j]);
            itomp(ONE, Gcd[m + j]);
        }

    lrs_clear_mp_vector(oD, d);
    lrs_clear_mp(Temp);
    lrs_clear_mp(mpone);
}

void FillConstraintRows_(lrs_dic *P, lrs_dat *Q, const game *g, const mpq_t *row_payoff_data, const mpq_t *col_payoff_data, int p1, int p2, int firstRow)
{
    const int MAXCOL = 1000; /* maximum number of columns */
    lrs_mp_vector num = lrs_alloc_mp_vector(MAXCOL);
    lrs_mp_vector den = lrs_alloc_mp_vector(MAXCOL);
    mpq_t x;
    mpq_init(x);
    int row, s, t;

    for (row = firstRow; row < firstRow + g->nstrats[p1]; row++)
    {
        mpz_set_ui(num[0], 0);
        mpz_set_ui(den[0], 1);
        s = row - firstRow;
        for (t = 0; t < g->nstrats[p2]; t++)
        {
            int idx = s * g->nstrats[p2] + t;
            int idx_ = t * g->nstrats[p2] + s;

            mpq_set(x, p1 == ROW ? row_payoff_data[idx] : col_payoff_data[idx_]); // TODO lol
            mpq_get_num(num[t+1], x);
            mpz_neg(num[t+1], num[t+1]);
            mpq_get_den(den[t+1], x);
        }
        mpz_set_ui(num[g->nstrats[p2] + 1], 1);
        mpz_set_ui(den[g->nstrats[p2] + 1], 1);
        lrs_set_row_mp_(P, Q, row, num, den, GE);
    }
}

void BuildRep_(lrs_dic *P, lrs_dat *Q, const game *g, const mpq_t *row_payoff_data, const mpq_t *col_payoff_data, int p1, int p2)
{
    long m = Q->m; /* number of inequalities      */
    long n = Q->n;

    if (p1 == 0)
    {
        FillConstraintRows_(P, Q, g, row_payoff_data, col_payoff_data, p1, p2, 1);
        FillNonnegativityRows(P, Q, g->nstrats[p1] + 1, g->nstrats[ROW] + g->nstrats[COL], n);
    }
    else
    {
        FillNonnegativityRows(P, Q, 1, g->nstrats[p2], n);
        FillConstraintRows_(P, Q, g, row_payoff_data, col_payoff_data, p1, p2, g->nstrats[p2] + 1); // 1 here
    }
    FillLinearityRow(P, Q, m, n);

    // TL added this to get first row of ones. (Is this necessary?)
    FillFirstRow(P, Q, n);
}

int solve_2(game *g, int rows, int cols, mpq_t *row_payoff_data, mpq_t *col_payoff_data, mpz_t *row_data, mpz_t *col_data)
{
    lrs_init("*lrsnash:");
    g->nstrats[0] = rows;
    g->nstrats[1] = cols;

    // init payoff

    lrs_dic *P1;      /* structure for holding current dictionary and indices */
    lrs_dat *Q1, *Q2; /* structure for holding problem data            */

    lrs_mp_vector output1; /* holds one line of output; ray,vertex,facet,linearity */
    lrs_mp_vector output2; /* holds one line of output; ray,vertex,facet,linearity */
    lrs_mp_matrix Lin;     /* holds input linearities if any are found             */
    lrs_mp_matrix A2orig;
    lrs_dic *P2orig; /* we will save player 2's dictionary in getabasis      */

    long *linindex; /* for faster restart of player 2                       */

    long col; /* output column index for dictionary                   */
    long startcol = 0;
    long prune = FALSE;  /* if TRUE, getnextbasis will prune tree and backtrack  */
    long numequilib = 0; /* number of nash equilibria found                      */
    long oldnum = 0;

    Q1 = lrs_alloc_dat("LRS globals"); /* allocate and init structure for static problem data */
    if (Q1 == NULL)
    {
        return 0;
    }

    Q1->nash = TRUE;
    Q1->n = g->nstrats[ROW] + 2;
    Q1->m = g->nstrats[ROW] + g->nstrats[COL] + 1;

    Q1->debug = Debug_flag;
    Q1->verbose = Verbose_flag;

    P1 = lrs_alloc_dic(Q1); /* allocate and initialize lrs_dic */
    if (P1 == NULL)
    {
        return 0;
    }

    BuildRep_(P1, Q1, g, row_payoff_data, col_payoff_data, 1, 0);

    output1 = lrs_alloc_mp_vector(Q1->n + Q1->m); /* output holds one line of output from dictionary     */

    /* allocate and init structure for player 2's problem data */
    Q2 = lrs_alloc_dat("LRS globals");
    if (Q2 == NULL)
    {
        return 0;
    }

    Q2->debug = Debug_flag;
    Q2->verbose = Verbose_flag;

    Q2->nash = TRUE;
    Q2->n = g->nstrats[COL] + 2;
    Q2->m = g->nstrats[ROW] + g->nstrats[COL] + 1;

    P2orig = lrs_alloc_dic(Q2); /* allocate and initialize lrs_dic */
    if (P2orig == NULL)
    {
        return 0;
    }
    BuildRep_(P2orig, Q2, g, row_payoff_data, col_payoff_data, 0, 1);
    A2orig = P2orig->A;

    output2 = lrs_alloc_mp_vector(Q1->n + Q1->m); /* output holds one line of output from dictionary     */

    linindex = calloc((P2orig->m + P2orig->d + 2), sizeof(long)); /* for next time */

    if (!lrs_getfirstbasis(&P1, Q1, &Lin, TRUE))
        return 1;

    if (Q1->homogeneous && Q1->hull)
        startcol++;
    col = startcol;

    do
    {
        prune = lrs_checkbound(P1, Q1);
        if (!prune && lrs_getsolution(P1, Q1, output1, col))
        {
            oldnum = numequilib;

            lrs_dic *P2;       /* This can get resized, cached etc. Loaded from P2orig */
            lrs_mp_matrix Lin; /* holds input linearities if any are found             */
            long col;          /* output column index for dictionary                   */
            long startcol = 0;
            long prune = FALSE; /* if TRUE, getnextbasis will prune tree and backtrack  */
            long nlinearity;
            long *linearity;

            long i, j;

            P2 = lrs_getdic(Q2);
            copy_dict(Q2, P2, P2orig);

            linearity = Q2->linearity;
            nlinearity = 0;
            for (i = Q1->lastdv + 1; i <= P1->m; i++)
            {
                if (!__zero(P1->A[P1->Row[i]][0]))
                {
                    j = Q1->inequality[P1->B[i] - Q1->lastdv];
                    if (Q1->nlinearity == 0 || j < Q1->linearity[0])
                        linearity[nlinearity++] = j;
                }
            }
            if (Q1->nlinearity > 0)
                linearity[nlinearity++] = Q1->linearity[0];

            for (i = 1; i < nlinearity; i++)
                reorder(linearity, nlinearity);

            Q2->nlinearity = nlinearity;
            Q2->polytope = FALSE;

            if (!lrs_getfirstbasis2(&P2, Q2, P2orig, &Lin, TRUE, linindex))
                goto sayonara;

            if (Q2->homogeneous && Q2->hull)
                startcol++;

            do
            {
                prune = lrs_checkbound(P2, Q2);
                col = 0;
                if (!prune && lrs_getsolution(P2, Q2, output2, col))
                {
                    long lrs_nashoutput1 = TRUE;
                    long i1;
                    long origin = TRUE;

                    for (i1 = 1; i1 < Q2->n; i1++)
                        if (!__zero(output2[i1]))
                            origin = FALSE;

                    if (origin)
                        lrs_nashoutput1 = FALSE;

                    if (lrs_nashoutput1)
                    {
                        for (i1 = 1; i1 < Q2->n; i1++)
                            prat_("", output2[i1], output2[0]);

                        for (i1 = 0; i1 < Q2->n; i1++)
                        {
                            __copy(col_data[i1], output2[i1]);
                        }

                        numequilib++;
                    }
                }
            } while (lrs_getnextbasis(&P2, Q2, prune));

        sayonara:
            lrs_free_dic(P2, Q2);

            if (numequilib > oldnum || Q1->verbose)
            {
                long lrs_nashoutput2 = TRUE;
                long i1;
                long origin = TRUE;

                for (i1 = 1; i1 < Q1->n; i1++)
                    if (!__zero(output1[i1]))
                        origin = FALSE;

                if (origin)
                    lrs_nashoutput2 = FALSE;

                if (lrs_nashoutput2)
                {
                    for (i1 = 1; i1 < Q1->n; i1++)
                        prat_("", output1[i1], output1[0]);

                    for (i1 = 0; i1 < Q1->n; i1++)
                    {
                        __copy(row_data[i1], output1[i1]);
                    }
                    goto done;
                }
            }
        }
    } while (lrs_getnextbasis(&P1, Q1, prune));

done:
    lrs_clear_mp_vector(output1, Q1->m + Q1->n);
    lrs_clear_mp_vector(output2, Q1->m + Q1->n);

    lrs_free_dic(P1, Q1); /* deallocate lrs_dic */
    lrs_free_dat(Q1);     /* deallocate lrs_dat */

    Q2->Qhead = P2orig; /* reset this or you crash free_dic */
    P2orig->A = A2orig; /* reset this or you crash free_dic */

    lrs_free_dic(P2orig, Q2); /* deallocate lrs_dic */
    lrs_free_dat(Q2);         /* deallocate lrs_dat */

    free(linindex);
    return 0;
}

void prat_(const char *name, lrs_mp Nin, lrs_mp Din)
{
    lrs_mp temp1, temp2;
    lrs_alloc_mp(temp1);
    lrs_alloc_mp(temp2);
    __copy(temp1, Nin);
    __copy(temp2, Din);
    reduce(temp1, temp2);
    lrs_clear_mp(temp1);
    lrs_clear_mp(temp2);
}