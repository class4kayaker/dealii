// ---------------------------------------------------------------------
//
// Copyright (C) 2019 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE.md at
// the top level directory of deal.II.
//
// ---------------------------------------------------------------------


// Test that the linfty_norm function returns the correct value
// for a block matrix.


#include <deal.II/lac/block_sparse_matrix.h>
#include <deal.II/lac/block_sparsity_pattern.h>

#include "../tests.h"



int
main()
{
  initlog();

  BlockSparsityPattern bsp(2, 2);
  // set sizes
  for (unsigned int i = 0; i < 2; ++i)
    for (unsigned int j = 0; j < 2; ++j)
      bsp.block(i, j).reinit(5, 5, 5);
  bsp.collect_sizes();

  // make a full matrix
  for (unsigned int row = 0; row < 10; ++row)
    for (unsigned int i = 0; i < 10; ++i)
      bsp.add(row, i);
  bsp.compress();

  BlockSparseMatrix<double> bsm(bsp);

  // Set all entries to 1
  for (unsigned int i = 0; i < bsm.m(); ++i)
    for (unsigned int j = 0; j < bsm.n(); ++j)
      {
        bsm.add(i, j, 1.0);
      }

  // Add 1 to 7 entries, chosen to make the linfty norm is based on values
  // across blocks
  bsm.add(0, 0, 1.0);
  bsm.add(3, 0, 1.0);
  bsm.add(5, 0, 1.0);
  bsm.add(3, 2, 1.0);
  bsm.add(3, 6, 1.0);
  bsm.add(3, 8, 1.0);
  bsm.add(5, 2, 1.0);

  const double accuracy            = 1e-12;
  const double correct_linfty_norm = 14.0;
  const double linfty_norm         = bsm.linfty_norm();

  Assert(std::fabs(linfty_norm - correct_linfty_norm) < accuracy,
         ExcInternalError());

  deallog << "OK" << std::endl;

  return 0;
}
