
#include "sharp_latlong.hpp"
#include<sharp_almhelpers.h>
#include<sharp_geomhelpers.h>
#include<sharp_lowlevel.h>
#include<stdexcept>

class SHT_LatLong::Impl
{
public:
	sharp_geom_info *geom_info;
	sharp_alm_info *alm_info;
	
	Impl(uint32_t width,uint32_t height,uint32_t lmax,uint32_t mmax)
	{
		sharp_make_fejer1_geom_info(height,width,0.0,1,width,&geom_info); //possible error here...stride needs to be nonzero?  Probably 1 or width...
		if(width*height != sharp_map_size(geom_info))
		{
			throw std::runtime_error("Error...size of latlong map data is not what was expected");
		}
		sharp_make_rectangular_alm_info(lmax,mmax,1,&alm_info);  //maybe needs to be stride 1?
	}
	~Impl()
	{
		sharp_destroy_alm_info(alm_info);
		sharp_destroy_geom_info(geom_info);
	}
};

SHT_LatLong::SHT_LatLong(uint32_t twidth,uint32_t theight,int tlmax,int tmmax):
	impl(new SHT_LatLong::Impl(twidth,theight,tlmax,tmmax)),
	td_width(twidth),
	td_height(theight),
	fd_lmax(tlmax),
	fd_mmax(tmmax)
{}


size_t SHT_LatLong::num_fd_samples() const
{
	return sharp_alm_count(impl->alm_info);
}

size_t SHT_LatLong::fd_index(int l,int m) const
{
	return sharp_alm_index(impl->alm_info,l,m);
}

size_t SHT_LatLong::num_td_samples() const
{
	return sharp_map_size(impl->geom_info);
}

size_t SHT_LatLong::td_index(int longitude,int colatitude) const
{
	return colatitude*td_width+longitude;
}
//TODO: apparently can do more than one in parallel which is cool I guess.
void SHT_LatLong::execute_transform(const double* td_samples,std::complex<double>* fd_samples) const
{
	double time;
	unsigned long long opcnt;
	
	double* fd_real_flat=reinterpret_cast<double*>(fd_samples);
	double* td=const_cast<double*>(td_samples);

	sharp_execute(SHARP_MAP2ALM,0,
		&fd_real_flat,
		&td,
		impl->geom_info,
		impl->alm_info,
		1,
		SHARP_DP,
		&time,&opcnt);
}
void SHT_LatLong::execute_inverse_transform(const std::complex<double>* fd_samples,double* td_samples) const
{
	double time;
	unsigned long long opcnt;

	double* fd_real_flat=reinterpret_cast<double*>(const_cast<std::complex<double>*>(fd_samples));
	double* td=td_samples;
	
	sharp_execute(SHARP_ALM2MAP,0,
		&fd_real_flat,
		&td_samples,
		impl->geom_info,
		impl->alm_info,
		1,
		SHARP_DP, //should sharp_add be an option here? No probably not.
		&time,&opcnt);
}


