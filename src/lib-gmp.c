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

    Q1 = lrs_alloc_dat("LRS globals"); /* allocate and init structure for problem data */
    if (Q1 == NULL)
    {
        return 0;
    }

    Q1->nash = TRUE;
    Q1->n = g->nstrats[ROW] + 2;
    Q1->m = g->nstrats[ROW] + g->nstrats[COL] + 1;

    Q1->debug = Debug_flag;
    Q1->verbose = Verbose_flag;

    // lrs_mp_vector col_data_copy = lrs_alloc_mp_vector(Q1->n + Q1->m);

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
        startcol++; /* col zero not treated as redundant   */
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
            long prune = FALSE;
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
                    if (Q2->verbose)
                        prat_(" \np1's obj value: ", P2->objnum, P2->objden);

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
                            *(col_data[i1]) = *output2[i1];
                        }

                        numequilib++;
                    }
                }
            } while (lrs_getnextbasis(&P2, Q2, prune));

        sayonara:
            lrs_free_dic(P2, Q2);

            if (numequilib > oldnum || Q1->verbose)
            {
                if (Q1->verbose)
                    prat_(" \np2's obj value: ", P1->objnum, P1->objden);

                long lrs_nashoutput2 = TRUE;
                long i1;
                long origin = TRUE;
                /* do not print the origin for either player */
                for (i1 = 1; i1 < Q1->n; i1++)
                    if (!__zero(output1[i1]))
                        origin = FALSE;

                if (origin)
                    lrs_nashoutput2 = FALSE;

                if (lrs_nashoutput2)
                {
                    // fprintf(lrs_ofp, "%ld ", 1L);
                    for (i1 = 1; i1 < Q1->n; i1++)
                        prat_("", output1[i1], output1[0]);

                    for (i1 = 0; i1 < Q1->n; i1++)
                    {
                        *(row_data[i1]) = *output1[i1];
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

    /* 2015.10.10  new code to clear P2orig */
    Q2->Qhead = P2orig; /* reset this or you crash free_dic */
    P2orig->A = A2orig; /* reset this or you crash free_dic */

    lrs_free_dic(P2orig, Q2); /* deallocate lrs_dic */
    lrs_free_dat(Q2);         /* deallocate lrs_dat */

    free(linindex);

    return 0;
}

void prat_(const char *name, lrs_mp Nin, lrs_mp Din)
/*print the long precision rational Nt/Dt  */
{
    lrs_mp Nt, Dt;
    __copy(Nt, Nin);
    __copy(Dt, Din);
    reduce(Nt, Dt);
}