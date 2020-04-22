// Copyright [2020] <Copyright Kevin, kevin.lau.gd@gmail.com>

#ifndef FT_INCLUDE_RISKMANAGEMENT_NOSELFTRADE_H_
#define FT_INCLUDE_RISKMANAGEMENT_NOSELFTRADE_H_

#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "Position.h"
#include "RiskManagement/RiskRuleInterface.h"
#include "TradingSystem.h"

namespace ft {

// 拦截自成交订单，检查相反方向的挂单
// 1. 市价单
// 2. 非市价单的其他订单，且价格可以成功撮合的
class NoSelfTradeRule : public RiskRuleInterface {
 public:
  NoSelfTradeRule(TradingSystem* ts, PositionManager* pos_mgr)
    : ts_(ts),
      pos_mgr_(pos_mgr) {}

  bool check(const Order* order) override {
    const auto pos = pos_mgr_->get_position(order->ticker);
    Direction opp_d = opp_direction(order->direction);  // 对手方
    const Order* pending_order;
    std::vector<Order> order_list;
    ts_->get_order_list(&order_list, order->ticker);

    for (const auto& o : order_list) {
      pending_order = &o;
      if (pending_order->direction != opp_d)
        continue;

      // 存在市价单直接拒绝
      if (pending_order->type == OrderType::MARKET)
        goto catch_order;

      if (order->direction == Direction::BUY) {
        if (order->price >= pending_order->price + 1e-5)
          goto catch_order;
      } else {
        if (order->price <= pending_order->price - 1e-5)
          goto catch_order;
      }
    }

    return true;

  catch_order:
    spdlog::error("[RiskMgr] Self trade! Ticker: {}. This Order: "
                  "[Direction: {}, Type: {}, Price: {:.2f}]. "
                  "Pending Order: [Direction: {}, Type: {}, Price: {:.2f}]",
                  order->ticker, to_string(order->direction), to_string(order->type),
                  order->price, to_string(pending_order->direction),
                  to_string(pending_order->type), pending_order->price);
    return false;
  }

 private:
  TradingSystem* ts_;
  PositionManager* pos_mgr_;
};

}  // namespace ft

#endif  // FT_INCLUDE_RISKMANAGEMENT_NOSELFTRADE_H_