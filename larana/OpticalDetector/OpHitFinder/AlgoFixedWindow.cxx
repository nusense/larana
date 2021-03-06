////////////////////////////////////////////////////////////////////////
//
//  AlgoFixedWindow source
//
////////////////////////////////////////////////////////////////////////

#include "fhiclcpp/ParameterSet.h"

#include "AlgoFixedWindow.h"

namespace pmtana{

  //*******************************************************************************
  AlgoFixedWindow::AlgoFixedWindow(const std::string name) : PMTPulseRecoBase(name)
  //*******************************************************************************
  {
    Reset();

    _index_start = 0;

    _index_end = 0;
  }

  //****************************************************************************************
  AlgoFixedWindow::AlgoFixedWindow(const fhicl::ParameterSet &pset,
  //AlgoFixedWindow::AlgoFixedWindow(const ::fcllite::PSet& pset,
				   const std::string name)
    : PMTPulseRecoBase(name)
  //****************************************************************************************
  {
    Reset();

    _index_start = pset.get<size_t>("StartIndex");

    _index_end = pset.get<size_t>("EndIndex");
  }

  //***************************************************************
  void AlgoFixedWindow::Reset()
  //***************************************************************
  {
    if(!(_pulse_v.size()))

      _pulse_v.push_back(_pulse);

    _pulse_v[0].reset_param();

  }

  //***************************************************************
  bool AlgoFixedWindow::RecoPulse(const Waveform_t& wf,
				  const PedestalMean_t& mean_v,
				  const PedestalSigma_t& sigma_v)
  //***************************************************************
  {
    this->Reset();

    if( _index_start >= wf.size() ) return true;

    _pulse_v[0].t_start = (double)(_index_start);

    _pulse_v[0].ped_mean  = mean_v.front();

    _pulse_v[0].ped_sigma = sigma_v.front();

    if(!_index_end)

      _pulse_v[0].t_end = (double)(wf.size() - 1);

    else if(_index_end < wf.size())

      _pulse_v[0].t_end = (double)_index_end;

    else

      _pulse_v[0].t_end = wf.size() - 1;

    _pulse_v[0].t_max = PMTPulseRecoBase::Max(wf, _pulse_v[0].peak, _index_start, _pulse_v[0].t_end);

    _pulse_v[0].peak -= mean_v.front();

    PMTPulseRecoBase::Integral(wf, _pulse_v[0].area, _index_start, _pulse_v[0].t_end);

    _pulse_v[0].area = _pulse_v[0].area - ( _pulse_v[0].t_end - _pulse_v[0].t_start + 1) * mean_v.front();

    return true;

  }

}
