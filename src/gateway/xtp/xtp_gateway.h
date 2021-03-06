// Copyright [2020] <Copyright Kevin, kevin.lau.gd@gmail.com>

#ifndef FT_SRC_GATEWAY_XTP_XTP_GATEWAY_H_
#define FT_SRC_GATEWAY_XTP_XTP_GATEWAY_H_

#include <xtp_quote_api.h>
#include <xtp_trader_api.h>

#include <memory>
#include <string>
#include <vector>

#include "gateway/gateway.h"
#include "gateway/xtp/_xtp_quote_api.h"
#include "gateway/xtp/xtp_common.h"
#include "gateway/xtp/xtp_trade_api.h"

namespace ft {

class XtpGateway : public Gateway {
 public:
  XtpGateway();

  bool Login(BaseOrderManagementSystem* oms, const Config& config) override;
  void Logout() override;
  bool Subscribe(const std::vector<std::string>& sub_list) override;

  bool SendOrder(const OrderRequest& order, uint64_t* privdata_ptr) override;
  bool CancelOrder(uint64_t order_id, uint64_t privdata) override;

  bool QueryContractList(std::vector<Contract>* result) override;
  bool QueryAccount(Account* result) override;
  bool QueryPositionList(std::vector<Position>* result) override;
  bool QueryTradeList(std::vector<Trade>* result) override;
  bool QueryOrderList();

 private:
  std::unique_ptr<XtpTradeApi> trade_api_;
  std::unique_ptr<XtpQuoteApi> quote_api_;
};

}  // namespace ft

#endif  // FT_SRC_GATEWAY_XTP_XTP_GATEWAY_H_
