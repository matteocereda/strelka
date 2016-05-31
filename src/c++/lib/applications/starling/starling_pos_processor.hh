// -*- mode: c++; indent-tabs-mode: nil; -*-
//
// Strelka - Small Variant Caller
// Copyright (c) 2009-2016 Illumina, Inc.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//

///
/// \author Chris Saunders
///

#pragma once

#include "gvcf_aggregator.hh"
#include "starling_shared.hh"
#include "starling_streams.hh"

#include "starling_common/starling_pos_processor_base.hh"



///
///
struct starling_pos_processor : public starling_pos_processor_base
{
    typedef starling_pos_processor_base base_t;

    starling_pos_processor(
        const starling_options& opt,
        const starling_deriv_options& dopt,
        const reference_contig_segment& ref,
        const starling_streams& streams);

    /// specify gvcf nocompress status of region
    void
    insert_nocompress_region(
        const known_pos_range2& range);

    void reset();

private:

    bool
    derived_empty() const override
    {
        return _nocompress_regions.empty();
    }

    bool
    is_save_pileup_buffer() override
    {
        return (_gvcfer && _gvcfer->is_phasing_block());
    }

    void
    process_pos_variants_impl(const pos_t pos) override
    {
        process_pos_indel_single_sample(pos,0);
        process_pos_snp_single_sample(pos,0);
    }

    void
    process_pos_snp_single_sample(
        const pos_t pos,
        const unsigned sample_no);

    void
    process_pos_snp_single_sample_impl(
        const pos_t pos,
        const unsigned sample_no);

    void
    process_pos_snp_single_sample_continuous(
        const pos_t pos,
        const unsigned sample_no);

    void
    process_pos_indel_single_sample(
        const pos_t pos,
        const unsigned sample_no);

    void process_pos_indel_single_sample_digt(
        const pos_t pos,
        const unsigned sampleId);
    void process_pos_indel_single_sample_continuous(
        const pos_t pos,
        const unsigned sampleId);

    void
    write_counts(const pos_range& output_report_range) const override;

    const starling_options& _opt;
    const starling_deriv_options& _dopt;
    const starling_streams& _streams;

    std::unique_ptr<gvcf_aggregator> _gvcfer;

    RegionTracker _nocompress_regions;
};
