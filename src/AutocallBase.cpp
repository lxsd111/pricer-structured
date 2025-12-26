#include "AutocallBase.hpp"
#include <algorithm>
#include <utility>

AutocallBase::AutocallBase(std::string underlying,
                           std::vector<double> observationTimes,
                           double spot0,
                           double notional,
                           double couponRate,
                           double callBarrier,
                           double protectionBarrier)
    : underlying_(std::move(underlying)),
      observationTimes_(std::move(observationTimes)),
      spot0_(spot0),
      notional_(notional),
      couponRate_(couponRate),
      callBarrier_(callBarrier),
      protectionBarrier_(protectionBarrier) {} // Plus de vérification stricte ici si on veut permettre des produits sans obs (ex: payoff immédiat)

double AutocallBase::terminalRedemption(double spotT) const {
    // Règle standard : Protection du capital si > barrière, sinon perte (PDI)
    if (spotT >= protectionBarrier_) {
        return notional_;
    }
    if (spot0_ <= 0.0) {
        return 0.0;
    }
    return notional_ * (spotT / spot0_);
}