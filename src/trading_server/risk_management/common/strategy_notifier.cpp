// Copyright [2020] <Copyright Kevin, kevin.lau.gd@gmail.com>

#include "trading_server/risk_management/common/strategy_notifier.h"

namespace ft {

void StrategyNotifier::OnOrderAccepted(const Order* order) {
  if (order->strategy_id[0] != 0) {
    OrderResponse rsp{};
    rsp.client_order_id = order->client_order_id;
    rsp.order_id = order->req.order_id;
    rsp.ticker_id = order->req.contract->ticker_id;
    rsp.direction = order->req.direction;
    rsp.offset = order->req.offset;
    rsp.original_volume = order->req.volume;
    rsp.error_code = NO_ERROR;
    rsp_redis_.publish(order->strategy_id, &rsp, sizeof(rsp));
  }
}

void StrategyNotifier::OnOrderTraded(const Order* order, const Trade* trade) {
  if (order->strategy_id[0] != 0) {
    OrderResponse rsp{};
    rsp.client_order_id = order->client_order_id;
    rsp.order_id = order->req.order_id;
    rsp.ticker_id = order->req.contract->ticker_id;
    rsp.direction = order->req.direction;
    rsp.offset = order->req.offset;
    rsp.original_volume = order->req.volume;
    rsp.traded_volume = order->traded_volume;
    rsp.this_traded = trade->volume;
    rsp.this_traded_price = trade->price;
    rsp.completed = order->canceled_volume + order->traded_volume == order->req.volume;
    rsp.error_code = NO_ERROR;
    rsp_redis_.publish(order->strategy_id, &rsp, sizeof(rsp));
  }
}

void StrategyNotifier::OnOrderCanceled(const Order* order, int canceled) {
  Trade tmp{};
  OnOrderTraded(order, &tmp);
}

void StrategyNotifier::OnOrderRejected(const Order* order, int error_code) {
  if (order->strategy_id[0] != 0) {
    OrderResponse rsp{};
    rsp.client_order_id = order->client_order_id;
    rsp.order_id = order->req.order_id;
    rsp.ticker_id = order->req.contract->ticker_id;
    rsp.direction = order->req.direction;
    rsp.offset = order->req.offset;
    rsp.original_volume = order->req.volume;
    rsp.completed = true;
    rsp.error_code = error_code;
    rsp_redis_.publish(order->strategy_id, &rsp, sizeof(rsp));
  }
}

}  // namespace ft
