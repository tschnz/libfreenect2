/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2014 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */

/** @file rgb_packet_stream_parser.h Parser classes for getting RGB packets from the stream. */

#ifndef RGB_PACKET_STREAM_PARSER_H_
#define RGB_PACKET_STREAM_PARSER_H_

#include <stddef.h>

#include "libfreenect2/config.h"
#include "rgb_packet_processor.h"
#include "../data_callback.h"

namespace libfreenect2
{

  /** Parser for getting an RGB packet from the stream. */
  class RgbPacketStreamParser : public DataCallback
  {
  public:
    RgbPacketStreamParser();
    virtual ~RgbPacketStreamParser();

    void setPacketProcessor(BaseRgbPacketProcessor *processor);

    virtual void onDataReceived(unsigned char *buffer, size_t length);

  private:
    size_t buffer_size_;
    RgbPacket packet_;
    BaseRgbPacketProcessor *processor_; ///< Parser implementation.
  };

} /* namespace libfreenect2 */
#endif /* RGB_PACKET_STREAM_PARSER_H_ */
