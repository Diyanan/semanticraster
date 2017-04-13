// ChangeDetectWrapper.h

#pragma once

namespace ChangeDetectWrapper {

	public ref class CDTWrapper
	{
	public:
		void Classifier(System::String^ pathTrian, System::String^ pathTest, System::String^ pathOutput, int method);
	};
}
