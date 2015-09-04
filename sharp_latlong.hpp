#ifndef SHARP_LATLONG_HPP
#define SHARP_LATLONG_HPP

#include<memory>
#include<cstdint>
#include<complex>
//header;

class SHT_LatLong
{
protected:
	class Impl;
	std::shared_ptr<Impl> impl;
public:
	const size_t td_width;
	const size_t td_height;
	const size_t fd_lmax;
	const size_t fd_mmax;
	
	SHT_LatLong(uint32_t width,uint32_t height,int lmax,int mmax);
	
	size_t num_td_samples() const;
	size_t num_fd_samples() const;
	
	size_t td_index(int longitude,int colatitude) const;
	size_t fd_index(int l,int m) const;
	
	void execute_transform(const double* td_samples,std::complex<double>* fd_samples) const;
	void execute_inverse_transform(const std::complex<double>* fd_samples,double* td_samples) const;
};

#endif 

#ifdef USAGE_EXAMPLE_SHARP_LATLONG

vector<double> diffuse_filter(const vector<double>& llin,size_t width=1024,size_t height=512)
{
	SHT_LatLong diffuse_transform(width,height,3,3); //do a lossy low-frequency projection to bottom 16 coefficients and back.
	vector<std::complex<double> > fd_coeffs(diffuse_transform.num_fd_samples());
	vector<double> diffuse_map(diffuse_transform.num_td_samples());

	diffuse_transform.execute_transform(llin.data(),fd_coeffs.data());
	diffuse_transform.execute_inverse_transform(fd_coeffs.data(),diffuse_map.data());
	return diffuse_map;
}

#endif

