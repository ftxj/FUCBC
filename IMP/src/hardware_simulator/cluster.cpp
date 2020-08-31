#include "hardware_simulator/cluster.hpp"
#include "error/assert.hpp"
Cluster::Cluster() {}

void Cluster::write_to_dst(std::vector<int> &data, LocalAddress &dst) {
    assert_msg(dst.on_cb, "TODO add write to reg support");
    int row = dst.address;
    for(int i = 0; i < crossbars_.size(); ++i) {
        crossbars_[i].w
    }
}

std::vector<int> Cluster::read_from_src(LocalAddress &src) {

}

std::vector<int> Cluster::read_from_reg_mask(std::vector<bool> &reg_mask) {

}

std::vector<bool> Cluster::generate_mask_from_address(LocalAddress &addr) {

}

Instruction Cluster::get_next_inst() {

}

void Cluster::load_instruction() {

}

void Cluster::exec_sum(std::vector<bool> &mask, LocalAddress &dst) {
    std::vector<std::vector<int>> sum_res(crossbars_.size());
    for(size_t i = 0; i < crossbars_.size(); ++i) {
        sum_res[i] = crossbars_[i].exec_sum(mask);
    }
    write_to_dst(dst);
}

void Cluster::exec_sub(std::vector<bool> &mask1, std::vector<bool> &mask2, LocalAddress &dst) {
    std::vector<std::vector<int>> sub_res(crossbars_.size());
    for(size_t i = 0; i < crossbars_.size(); ++i) {
        sub_res[i] = crossbars_[i].exec_sub(mask1, mask2);
    }
    write_to_dst(sub_res, dst);
}

void Cluster::exec_dot(std::vector<bool> &mask1, std::vector<bool> &reg_mask2, LocalAddress &dst) {
    std::vector<std::vector<int>> dot_res(crossbars_.size());
    for(size_t i = 0; i < crossbars_.size(); ++i) {
        dot_res[i] = crossbars_[i].exec_dot(mask1, read_from_reg_mask(reg_mask2));
    }
    write_to_dst(dot_res, dst);
}

void Cluster::exec_mul(LocalAddress &src1, LocalAddress &src2, LocalAddress &dst) {
    std::vector<std::vector<int>> mul_res(crossbars_.size());
    for(size_t i = 0; i < crossbars_.size(); ++i) {
        auto input = crossbars_[i].read_from_address(src1);
        auto mask = generate_mask_from_address(src2);
        mul_res[i] = crossbars_[i].exec_mul(input, mask);
    }
    write_to_dst(mul_res, dst);
}

void Cluster::exec_shift_r(LocalAddress &src, LocalAddress &dst, int imm) {
    // TODO
    std::vector<std::vector<int>> shift_r_res(crossbars_.size());
    for(size_t i = 0; i < crossbars_.size(); ++i) {
        auto input = crossbars_[i].read_from_src(src1);
        for(auto l : input) {
            shift_r_res[i].push_back(l>>imm);
        }
    }
    write_to_dst(shift_r_res, dst);
}

void Cluster::exec_shift_l(LocalAddress &src, LocalAddress &dst, int imm) {
    // TODO
    std::vector<std::vector<int>> shift_r_res(crossbars_.size());
    for(size_t i = 0; i < crossbars_.size(); ++i) {
        auto input = crossbars_[i].read_from_src(src1);
        for(auto l : input) {
            shift_r_res[i].push_back(l<<imm);
        }
    }
    write_to_dst(shift_r_res, dst);
}


void Cluster::exec_mask(LocalAddress &src, LocalAddress &dst, int imm) {
    // TODO
    std::vector<std::vector<int>> shift_r_res(crossbars_.size());
    for(size_t i = 0; i < crossbars_.size(); ++i) {
        auto input = crossbars_[i].read_from_src(src1);
        for(auto l : input) {
            shift_r_res[i].push_back(l & imm);
        }
    }
    write_to_dst(shift_r_res, dst);
}

void Cluster::exec_lut(LocalAddress &src, LocalAddress &dst) {
    // TODO
    auto key = read_from_src(src);
    auto res = lut_.lookup(key);
    write_to_dst(res, dst);
}

void Cluster::exec_mov(LocalAddress &src, LocalAddress &dst) {
    // TODO
    std::vector<std::vector<int>> data(crossbars_.size());
    for(size_t i = 0; i < crossbars_.size(); ++i) {
        data[i] = crossbars_[i].read_from_src(src1);
    }
}

void Cluster::exec_movi(LocalAddress &dst, int imm) {
    // TODO, mov imm to reg file

}

void Cluster::exec_movs(LocalAddress &src, LocalAddress &dst, std::vector<bool> &mask) {
    // TODO, mov imm to reg file
    
}

std::vector<int> Cluster::read_data(int index, int row) {
    return crossbars_[index].read_row(row);
}

std::vector<int> Cluster::reduce_sum_exec(Instruction &inst) {

}

void Cluster::exec(Instruction inst) {

}

void Cluster::run(int clock, bool global_sync, std::vector<int> &comm_data, bool &need_sync) {
    if(begin_clock_ + inst_running_.cycles_ == clock || global_sync) {
        exec(inst_running_);
        inst_running_ = get_next_inst();
        begin_clock_ = clock;
    }
    if(inst_running_.op_code_ == Instruction::REDUCE_SUM) {
        comm_data = reduce_sum_exec(inst);
        need_sync = true;
    }
}

void Cluster::global_write(GlobalAddress &addr, std::vector<int> data) {
    int 
}