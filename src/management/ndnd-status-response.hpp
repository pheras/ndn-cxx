/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2013 Regents of the University of California.
 * See COPYING for copyright and distribution information.
 */

#ifndef NDN_MANAGEMENT_NDND_STATUS_RESPONSE_HPP
#define NDN_MANAGEMENT_NDND_STATUS_RESPONSE_HPP

#include "../encoding/block.hpp"
#include "../encoding/tlv-ndnd.hpp"

namespace ndn {
namespace ndnd {

class StatusResponse {
public:
  StatusResponse()
    : code_(0)
  {
  }

  StatusResponse(uint32_t code, const std::string &info)
    : code_(code)
    , info_(info)
  {
  }
  
  inline uint32_t
  getCode() const;

  inline void
  setCode(uint32_t code);

  inline const std::string &
  getInfo() const;

  inline void
  setInfo(const std::string &info);

  inline const Block&
  wireEncode() const;

  inline void
  wireDecode(const Block &block);
  
private:
  uint32_t code_;
  std::string info_;

  mutable Block wire_;
};

inline uint32_t
StatusResponse::getCode() const
{
  return code_;
}

inline void
StatusResponse::setCode(uint32_t code)
{
  code_ = code;
  wire_.reset();
}

inline const std::string &
StatusResponse::getInfo() const
{
  return info_;
}

inline void
StatusResponse::setInfo(const std::string &info)
{
  info_ = info;
  wire_.reset();
}


inline const Block&
StatusResponse::wireEncode() const
{
  if (wire_.hasWire())
    return wire_;

  wire_ = Block(tlv::ndnd::StatusResponse);
  wire_.push_back
    (nonNegativeIntegerBlock(tlv::ndnd::StatusCode, code_));

  if (!info_.empty())
    {
      wire_.push_back
        (dataBlock(tlv::ndnd::StatusText, info_.c_str(), info_.size()));
    }
  
  wire_.encode();  
  return wire_;
}

inline void
StatusResponse::wireDecode(const Block &wire)
{
  wire_ = wire;
  wire_.parse();

  code_ = readNonNegativeInteger(wire_.get(tlv::ndnd::StatusCode));

  Block::element_const_iterator val = wire_.find(tlv::ndnd::StatusText);
  if (val != wire_.elements_end())
    {
      info_.assign(reinterpret_cast<const char*>(val->value()), val->value_size());
    }
}

inline std::ostream&
operator << (std::ostream &os, const StatusResponse &status)
{
  os << status.getCode() << " " << status.getInfo();
  return os;
}

} // namespace ndnd
} // namespace ndn

#endif // NDN_MANAGEMENT_NDND_STATUS_RESPONSE_HPP
