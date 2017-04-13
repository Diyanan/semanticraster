#pragma once

namespace VerifyFeatureSet
{
	struct SampleStatistics
	{
		double max;
		double min;
		double mean;
		double stddev;
		double variance;
		int number;
		//bool hasHistogram;
		bool isValid;
		int buckets;
		float* histogram;//存放的是归一化后的直方图
	};
}