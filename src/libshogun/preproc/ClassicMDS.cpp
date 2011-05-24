/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2011 Sergey Lisitsyn
 * Copyright (C) 2011 Berlin Institute of Technology and Max-Planck-Society
 */

#include "preproc/ClassicMDS.h"
#include "lib/lapack.h"
#include "lib/common.h"
#include "lib/Mathematics.h"
#include "lib/io.h"
#include "distance/EuclidianDistance.h"
#include "lib/Signal.h"

using namespace shogun;

CClassicMDS::CClassicMDS() : CSimplePreProc<float64_t>()
{
}

CClassicMDS::~CClassicMDS()
{
}

bool CClassicMDS::init(CFeatures* data)
{
	CSimpleFeatures<float64_t>* pdata = (CSimpleFeatures<float64_t>*) data;
	CDistance* distance = new CEuclidianDistance(pdata,pdata);
	int32_t N = pdata->get_num_features();

	int32_t i,j;

	float64_t* D_matrix = new float64_t[N*N];
	float64_t* Ds_matrix = new float64_t[N*N];
	distance->get_distance_matrix(&D_matrix,&N,&N);

	cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,1.0,D_matrix,N,D_matrix,N,0.0,Ds_matrix,N);

	float64_t* I_matrix = new float64_t[N*N];
	for (i=0;i<N;i++)
		for (j=0;j<N;j++)
			I_matrix = (i==j) ? 1.0 - 1.0/n : 0.0;

	cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,1.0,I_matrix,N,Ds_matrix,N,0.0,D_matrix,N);
	cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,-0.5,D_matrix,N,I_matrix,N,0.0,Ds_matrix,N);

	//wrap_dsyev()

	return true;
}

void CClassicMDS::cleanup()
{

}

float64_t* CClassicMDS::apply_to_feature_matrix(CFeatures* f)
{
	return 0;
}

float64_t* CClassicMDS::apply_to_feature_vector(float64_t* f, int32_t &len)
{
	return 0;
}
