#include "CliquetBase.hpp"
#include <utility>

CliquetBase::CliquetBase(std::string underlying,
                         std::vector<double> observationTimes,
                         double spot0,
                         double notional)
    : underlying_(std::move(underlying)),
      observationTimes_(std::move(observationTimes)),
      spot0_(spot0),
      notional_(notional) {}

// C'est ici qu'on fait l'adaptation : un cliquet paie 1 seul flux à la fin.
std::vector<CashFlow> CliquetBase::cashFlows(const std::vector<double>& path) const {
    // On appelle la méthode virtuelle des enfants (MaxReturn, CappedCoupons)
    double amount = payoffImpl(path);

    // La date de paiement est la dernière date d'observation
    double payTime = observationTimes_.empty() ? 0.0 : observationTimes_.back();

    return {{amount, payTime}};
}

// Supprimez les méthodes `payoff`, `payoffAndPayTime` et `terminalRedemption` (version charting)
// qui sont obsolètes avec la nouvelle architecture.