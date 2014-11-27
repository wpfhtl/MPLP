/*
 *  muldim_arr.h
 *  mplp
 *
 *  Created by Amir Globerson on 6/24/08.
 *  Copyright 2008 MIT. All rights reserved.
 *
 */
#ifndef MPLP_MULDIM_ARR_H
#define MPLP_MULDIM_ARR_H

#include <vector>
#include <iostream>
#include <string.h> 

#include <MPLP/matrix.h>


#define huge 1e40
#define BASE2DEC_PAIR(x1,x2,base1,base2) (x1*base2+x2)

class MulDimArr 
{
public:
	std::vector<int> m_base_sizes;
	int m_n_prodsize;
	double *m_dat;
	double *m_ep;
	
	// Initialize to all zero
	MulDimArr(std::vector<int> & base_sizes);
	
	// Copy constructor
	MulDimArr(const MulDimArr & v);
	
	MulDimArr() 
	{
		m_n_prodsize = 0;
		m_dat = NULL;
	};
	~MulDimArr()
	{
		if (m_dat!=NULL)
			delete [] m_dat;
	}
	
	MulDimArr & operator=(const MulDimArr & v);
	MulDimArr & operator=(double val);
	MulDimArr & operator*=(double val);
	MulDimArr & operator+=(MulDimArr & v);
	MulDimArr & operator-=(MulDimArr & v);
	double & operator[](int i) {return m_dat[i];}
	const double & operator[](int i) const {return m_dat[i];}
	double Max(int &max_at) const;
	double Entropy(void) const;
	double Entropy_over_free_variables(const std::vector<int> &, const std::vector<int> &) const;
	void print(void) const;
	void print_with_inds(void) const;
	void Write(std::ofstream & ofs);
	void Read(std::ifstream & ifs);
	
	inline int GetFlatInd(std::vector<int> & base_inds) const;
	inline int GetFlatIndFromBig(std::vector<int> big_base_inds, std::vector<int> inds_in_big) const;
	int GetFlatIndFromBigSpecial(std::vector<int> & big_base_inds, std::vector<int> & inds_in_big) const;

	double GetVal(std::vector<int> & indices) const;
	MulDimArr Expand(std::vector<int> & var_sizes_big, std::vector<int> & inds_in_big);
	void ExpandAndAdd(MulDimArr & big_to_add_to, std::vector<int> & inds_of_small_in_big);
	void ExpandAndSubtract(MulDimArr & big_to_add_to, std::vector<int> & inds_of_small_in_big);

	inline void BaseInc(std::vector<int> & inds) const;
	inline void BaseIncSpecial(std::vector<int> & inds) const;

	void GetInds(int, std::vector<int> &) const;    //for decoding purposes

	void  max_into_multiple_subsets_special(std::vector<std::vector<int> > & all_subset_inds, std::vector<MulDimArr> & all_maxes) const;
	double max_over_free_variables(const std::vector<int> &, std::vector<int> &) const;
	double gap_over_free_variables(const std::vector<int> &, std::vector<int> &, double &, double &) const;
private:
	double _max_over_free_variables(int, int, int, int, const std::vector<int> &, std::vector<int> &) const;
	void _Entropy_over_free_variables(int, int, int, int, const std::vector<int> &, const std::vector<int> &, double &, double &) const;
};

// Return the index in the flat multi-dimensional array corresponding to
// the given multi-index
// NOTE: Since this function is called to get an intersection index out of a big index,
// as long as we don't use intersections of more than two, we only need the cases given here.
inline int MulDimArr::GetFlatIndFromBigSpecial(std::vector<int> & big_base_inds, std::vector<int> & inds_in_big) const{
	int y,ind1,ind2;
	
	switch (inds_in_big.size())
	{
		case 1:
			y = big_base_inds[inds_in_big[0]];
			break;
		case 2:
//			cout << "Not good here too" << endl;
			ind1 = inds_in_big[0];
			ind2 = inds_in_big[1];
			y = BASE2DEC_PAIR(big_base_inds[ind1],big_base_inds[ind2],m_base_sizes[0],m_base_sizes[1]);
			break;
		default:
		    std::cout << "GetFlatIndFromBigSpecial problem" << std::endl;
			break;
	}
	return y;
}

inline void MulDimArr::BaseIncSpecial(std::vector<int> & inds) const{
	switch (inds.size())
	{
		case 1:
			inds[0]++;
			break;
		case 2:
			inds[1]++;
			if (inds[1]==m_base_sizes[1])
			{
				inds[1] =0;
                inds[0]++;
            }	
			break;	
		case 3:
			inds[2]++;
			if (inds[2]==m_base_sizes[2])
			{
				inds[2]=0;
				inds[1]++;
				if (inds[1]==m_base_sizes[1])
				{
					inds[1]=0;
					inds[0]++;
				}
			}
			break;
		default: 
			for(int i=inds.size()-1; i >= 0; i--) {
				inds[i]++;
				if(inds[i] < m_base_sizes[i])
					break;
				inds[i]=0;
			}
	}
}

#endif
