/*
 * (C) Copyright 2017-2018 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef UFO_OBSSEAICETHICKNESSTLAD_H_
#define UFO_OBSSEAICETHICKNESSTLAD_H_

#include <ostream>
#include <string>

#include <boost/scoped_ptr.hpp>

#include "oops/base/Variables.h"
#include "oops/interface/LinearObsOperBase.h"
#include "ufo/ObsSpace.h"
#include "util/ObjectCounter.h"
#include "util/Logger.h"

// Forward declarations
namespace util {
  class DateTime;
}

namespace ufo {
  class GeoVaLs;
  class ObsBias;
  class ObsBiasIncrement;
  class ObsVector;

// -----------------------------------------------------------------------------
/// Sea-ice fraction observation for  model.
template <typename MODEL>
class ObsSeaIceThicknessTLAD : public oops::LinearObsOperBase<MODEL>, 
                              private util::ObjectCounter<ObsSeaIceThicknessTLAD<MODEL>> {
public:
  static const std::string classname() {return "ufo::ObsSeaIceThicknessTLAD";}

  ObsSeaIceThicknessTLAD(const ObsSpace &, const eckit::Configuration &);    
  virtual ~ObsSeaIceThicknessTLAD();

  // Obs Operators
  void setTrajectory(const GeoVaLs &, const ObsBias &);
  void obsEquivTL(const GeoVaLs &, ObsVector &, const ObsBiasIncrement &) const;
  void obsEquivAD(GeoVaLs &, const ObsVector &, ObsBiasIncrement &) const;

  // Other
  const oops::Variables & variables() const {return *varin_;}

  int & toFortran() {return keyOperSeaIceThickness_;}
  const int & toFortran() const {return keyOperSeaIceThickness_;}

private:
  void print(std::ostream &) const;
  F90hop keyOperSeaIceThickness_;
  boost::scoped_ptr<const oops::Variables> varin_;
};

// -----------------------------------------------------------------------------
template <typename MODEL>
ObsSeaIceThicknessTLAD<MODEL>::ObsSeaIceThicknessTLAD(const ObsSpace & odb, const eckit::Configuration & config)
  : keyOperSeaIceThickness_(0), varin_()
{
  const eckit::Configuration * configc = &config;
  ufo_seaicethick_setup_f90(keyOperSeaIceThickness_, &configc);
  const std::vector<std::string> vv{"ice_concentration", "ice_thickness"};
  varin_.reset(new oops::Variables(vv));
  oops::Log::trace() << "ObsSeaIceThicknessTLAD created" << std::endl;
}

// -----------------------------------------------------------------------------
template <typename MODEL>
ObsSeaIceThicknessTLAD<MODEL>::~ObsSeaIceThicknessTLAD() {
  oops::Log::trace() << "ObsSeaIceThicknessTLAD destrcuted" << std::endl;
}

// -----------------------------------------------------------------------------
template <typename MODEL>
void ObsSeaIceThicknessTLAD<MODEL>::setTrajectory(const GeoVaLs & geovals, const ObsBias & bias) {
  ufo_seaicethick_settraj_f90(keyOperSeaIceThickness_, geovals.toFortran());
}

// -----------------------------------------------------------------------------
template <typename MODEL>
void ObsSeaIceThicknessTLAD<MODEL>::obsEquivTL(const GeoVaLs & geovals, ObsVector & ovec,
                               const ObsBiasIncrement & bias) const {
  ufo_seaicethick_eqv_tl_f90(keyOperSeaIceThickness_, geovals.toFortran(), ovec.toFortran());
}

// -----------------------------------------------------------------------------
template <typename MODEL>
void ObsSeaIceThicknessTLAD<MODEL>::obsEquivAD(GeoVaLs & geovals, const ObsVector & ovec,
                               ObsBiasIncrement & bias) const {
  ufo_seaicethick_eqv_ad_f90(keyOperSeaIceThickness_, geovals.toFortran(), ovec.toFortran());
}

// -----------------------------------------------------------------------------
template <typename MODEL>
void ObsSeaIceThicknessTLAD<MODEL>::print(std::ostream & os) const {
  os << "ObsSeaIceThicknessTLAD::print not implemented" << std::endl;
}
// -----------------------------------------------------------------------------

}  // namespace ufo
#endif  // UFO_OBSSEAICETHICKNESSTLAD_H_