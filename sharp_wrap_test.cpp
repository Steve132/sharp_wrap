#include "sharp_latlong.hpp"
#include<vector>
#include<iostream>
using namespace std;

vector<double> diffuse_filter(const vector<double>& llin,size_t width=1024,size_t height=512)
{
	SHT_LatLong diffuse_transform(width,height,3,3); //do a lossy low-frequency projection to bottom 16 coefficients and back.
	vector<std::complex<double> > fd_coeffs(diffuse_transform.num_fd_samples());
	vector<double> diffuse_map(diffuse_transform.num_td_samples());

	diffuse_transform.execute_transform(llin.data(),fd_coeffs.data());
	diffuse_transform.execute_inverse_transform(fd_coeffs.data(),diffuse_map.data());
	return diffuse_map;
}

int main(int argc,char** argv)
{
	std::vector<double> constin(1024*512,1.0);
	std::vector<double> out=diffuse_filter(constin,1024,512);
	for(int i=0;i<10;i++)
	{
		cout << out[i] << "," << endl;
	}
	return 0;
}
