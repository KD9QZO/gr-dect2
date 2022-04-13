/* -*- c++ -*- */
/*
 * Copyright 2015 Pavel Yazev <pyazev@gmail.com>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_DECT2_PACKET_RECEIVER_IMPL_H
#define INCLUDED_DECT2_PACKET_RECEIVER_IMPL_H

#include <dect2/packet_receiver.h>


#define MAX_PARTS			(8)						/* Maximum number of DECT parts to be tracked */
#define SMPL_BUF_LEN		(32 * 4)
#define TIME_TOL			(10)					/* Time tolerance */
#define INTER_SLOT_TIME		(480 * 4)
#define INTER_FRAME_TIME	(INTER_SLOT_TIME * 24)
#define S_FIELD_BITS		(32)
#define P32_D_FIELD_BITS	(388)
#define RFP_SYNC_FIELD		(0xAAAAE98A)



namespace gr {
namespace dect2 {

class packet_receiver_impl: public packet_receiver {
private:
	typedef enum {
		_RFP_,
		_PP_
	} part_type;

	part_type d_part_type;

	/**
	 * \brief Buffer to save demodulated bits
	 *
	 * This is a buffer to save demodulated bits. The input signal has four samples per bits. \n
	 * We save bits related to \e null sample in \e null element, bits related to first sample in first element, and so
	 * on. Each element in this array \e could (should?) be considered as a circular buffer.
	 */
	uint32_t d_rx_bits_buf[4];
	unsigned d_rx_bits_buf_index;

	enum {
		_WAIT_BEGIN_,
		_WAIT_END_,
		_POST_WAIT_
	} d_sync_state;

	uint32_t d_begin_pos;
	uint32_t d_end_pos;
	uint32_t d_post_wait_cnt;

	float d_smpl_buf[SMPL_BUF_LEN];
	uint32_t d_smpl_buf_index;

	uint32_t d_smpl_cnt;
	uint32_t d_out_bit_cnt;
	uint64_t d_inc_smpl_cnt;				/* Incoming samples counter */

	uint64_t d_part_time[MAX_PARTS];
	uint32_t d_part_seq[MAX_PARTS];
	uint32_t d_part_activity;

	int32_t d_cur_part_rx_id;

	int d_decimation;

	int decimation() const {
		return (d_decimation);
	}

	void set_decimation(int decimation) {
		d_decimation = decimation;
		set_relative_rate(1.0 / decimation);
	}

	int fixed_rate_ninput_to_noutput(int ninput);
	int fixed_rate_noutput_to_ninput(int noutput);

	/**
	 * \brief Check for DECT part activity
	 *
	 * \return The RX ID of the part which no longer has any activity; otherwise, \b -1 indicates activity is present.
	 * \retval	-1	Activity was detected on the DECT part
	 */
	int check_part_activity(void);

	/**
	 * @brief Detect and register any DECT parts that are on-air
	 *
	 * If there are several DECT parts on-air, we need to keep track of each in a correct/proper way. This function does
	 * this by taking into account the time intervals (based on the incoming sample counter) between received bursts.
	 *
	 * \return The DECT part RX ID, if the appropriate part is found or a new one is assigned; otherwise, return \b -1
	 * \retval	-1	No DECT part was found
	 */
	int register_part(void);
	int find_best_smpl_point(void);

public:
	packet_receiver_impl();
	~packet_receiver_impl();

	/* Where all the action really happens */
	void forecast(int noutput_items, gr_vector_int &ninput_items_required);

	int general_work(int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items);
};


}	/* namespace dect2 */
}	/* namespace gr */


#endif	/* !INCLUDED_DECT2_PACKET_RECEIVER_IMPL_H */
