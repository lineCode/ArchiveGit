#pragma once

template <class T>
class CProxy_ICtpEvents : public IConnectionPointImpl<T, &DIID__ICtpEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_FileSaved(long lValue)
	{
		CComVariant varResult;
		CComVariant* pvars = new CComVariant[1];

		int nConnections = m_vec.GetSize();
		for (int nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			T* pT = static_cast<T*>(this);
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch)
			{
				VariantClear(&varResult);
				pvars[0] = lValue;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}

		delete [] pvars;
		return varResult.scode;
	}
};
