#include "gtest/gtest.h"
#include "paxosphil/messages/messages_all.hpp"
#include <variant>

using namespace std;

enum class mtype_t {
    pm_cp_fork_action_t,
    pm_pc_key_t,
    pm_pa_prepare_t,
    pm_ap_promise_t,
    pm_pa_accept_t,
    pm_al_accepted_t,
    pm_lc_verdict_t,
    nan
};

struct caller_t {
#define def_op_rndpths(__TYPENAME) mtype_t operator()(const __TYPENAME &) {return mtype_t::__TYPENAME;}

    def_op_rndpths(pm_cp_fork_action_t)
    def_op_rndpths(pm_pc_key_t)
    def_op_rndpths(pm_pa_prepare_t)
    def_op_rndpths(pm_ap_promise_t)
    def_op_rndpths(pm_pa_accept_t)
    def_op_rndpths(pm_al_accepted_t)
    def_op_rndpths(pm_lc_verdict_t)
    mtype_t operator()(auto) {return mtype_t::nan;}

#undef def_op_rndpths
};

TEST(PaxosPhilMessageSerialization, pm_cp_fork_action_t) {
    pm_cp_fork_action_t fa;
    fa.fork_id = 4;
    fa.fork_action = fork_action_t::put;

    auto serialized = fa.serialize();
    EXPECT_STREQ("C_REQ_FORK 4 1", serialized.data());

    auto deserialized = deserialize(serialized);
    ASSERT_EQ( visit(caller_t(), deserialized), mtype_t::pm_cp_fork_action_t );

    auto msg {get<pm_cp_fork_action_t>(deserialized)};
    EXPECT_EQ(msg.fork_id, 4);
    EXPECT_EQ(msg.fork_action, fork_action_t::put);
}

TEST(PaxosPhilMessageSerialization, pm_pc_key_t) {
    pm_pc_key_t key;
    key.fork_id = 2;
    key.n_prep_fork_id = 777;

    auto serialized {key.serialize()};
    EXPECT_STREQ("P_RSP_KEY 2 777", serialized.data());

    auto deserialized {deserialize(serialized)};
    ASSERT_EQ( visit(caller_t{}, deserialized), mtype_t::pm_pc_key_t );

    auto key_deserialized {get<pm_pc_key_t>(deserialized)};
    EXPECT_EQ(key_deserialized.fork_id, 2);
    EXPECT_EQ(key_deserialized.n_prep_fork_id, 777);
}

TEST(PaxosPhilMessageSerialization, pm_pa_prepare_t) {
    pm_pa_prepare_t prep;
    prep.fork_id = 299;
    prep.n_prep_fork_id = 2;

    auto serialized{prep.serialize()};
    EXPECT_STREQ("P_PREPARE 299 2", serialized.data());

    auto deserialized{deserialize(serialized)};
    ASSERT_EQ( mtype_t::pm_pa_prepare_t, visit(caller_t{}, deserialized) );

    auto prep_deserialized {get<pm_pa_prepare_t>(deserialized)};
    EXPECT_EQ(prep_deserialized.fork_id, 299);
    EXPECT_EQ(prep_deserialized.n_prep_fork_id, 2);
}

TEST(PaxosPhilMessageSerialization, pm_ap_promise_t) {
    pm_ap_promise_t promise_nack;
    promise_nack.promise_type = promise_type_t::nack;

    pm_ap_promise_t promise;
    promise.promise_type = promise_type_t::promise;
    promise.fork_id = 4;
    promise.n_prep_fork_id = 2;

    pm_ap_promise_t promise_value;
    promise_value.promise_type = promise_type_t::promise_have_value;
    promise_value.fork_id = 3;
    promise_value.n_prep_fork_id = 4;
    promise_value.fork_action = fork_action_t::take; /* 0 */

    auto serialized_nack {promise_nack.serialize()};
    auto serialized      {promise.serialize()};
    auto serialized_value{promise_value.serialize()};
    EXPECT_STREQ(serialized_nack.data(), "A_PROMISE NACK");
    EXPECT_STREQ(serialized.data(), "A_PROMISE 4 2");
    EXPECT_STREQ(serialized_value.data(), "A_PROMISE 3 4 0");

    auto deserialized_nack {deserialize(serialized_nack)};
    auto deserialized      {deserialize(serialized)};
    auto deserialized_value{deserialize(serialized_value)};
    auto vis ([&](auto& des) -> bool {return {visit(caller_t{}, des) == mtype_t::pm_ap_promise_t} ;} );
    ASSERT_TRUE(vis(deserialized_nack)
            &&  vis(deserialized)
            &&  vis(deserialized_value));

    auto get ( [&](auto& des) {return std::get<pm_ap_promise_t>(des);} );
    auto prnack {get(deserialized_nack)};
    auto pr     {get(deserialized)};
    auto prvalue{get(deserialized_value)};

    EXPECT_EQ( prnack.promise_type, promise_type_t::nack);
    EXPECT_EQ(     pr.promise_type, promise_type_t::promise);
    EXPECT_EQ(prvalue.promise_type, promise_type_t::promise_have_value);

    EXPECT_EQ(     pr.fork_id, 4);
    EXPECT_EQ(prvalue.fork_id, 3);

    EXPECT_EQ(     pr.n_prep_fork_id,2);
    EXPECT_EQ(prvalue.n_prep_fork_id,4);

    EXPECT_EQ(prvalue.fork_action, fork_action_t::take);
}

TEST(PaxosPhilMessageSerialization, pm_pa_accept_t) {
    pm_pa_accept_t acc;
    acc.fork_id        = 2;
    acc.n_prep_fork_id = 4;
    acc.fork_action    = fork_action_t::put; /* 1 */
    acc.client_port    = 6004;
    acc.verdict        = verdict_t::approved; /* 1 */

    auto serialized{acc.serialize()};
    EXPECT_STREQ(serialized.data(), "P_ACCEPT 2 4 1 6004 1");

    auto deserialized{ deserialize(serialized) };
    ASSERT_EQ(visit(caller_t{}, deserialized), mtype_t::pm_pa_accept_t);

    auto acc_deserialized{get<pm_pa_accept_t>(deserialized)};

    EXPECT_EQ(acc_deserialized.fork_id, 2);
    EXPECT_EQ(acc_deserialized.n_prep_fork_id, 4);
    EXPECT_EQ(acc_deserialized.fork_action, fork_action_t::put);
    EXPECT_EQ(acc_deserialized.client_port, 6004);
    EXPECT_EQ(acc_deserialized.verdict, verdict_t::approved);
}

TEST(PaxosPhilMessageSerialization, pm_al_accepted_t) {
    pm_al_accepted_t accd;
    accd.fork_id        = 2;
    accd.n_prep_fork_id = 4;
    accd.client_port    = 60003;
    accd.verdict        = verdict_t::denied; /* 0 */

    auto serialized{accd.serialize()};
    EXPECT_STREQ(serialized.data(), "A_ACCEPTED 2 4 60003 0");

    auto deserialized{deserialize(serialized)};
    ASSERT_EQ( visit(caller_t{}, deserialized), mtype_t::pm_al_accepted_t );

    auto accd_deserialized{get<pm_al_accepted_t>(deserialized)};
    EXPECT_EQ(accd_deserialized.fork_id, 2);
    EXPECT_EQ(accd_deserialized.n_prep_fork_id, 4);
    EXPECT_EQ(accd_deserialized.client_port, 60003);
    EXPECT_EQ(accd_deserialized.verdict, verdict_t::denied);
}

TEST(PaxosPhilMessageSerialization, pm_lc_verdict_t) {
    pm_lc_verdict_t verd;
    verd.fork_id        = 2;
    verd.n_prep_fork_id = 4;
    verd.verdict        = verdict_t::approved;

    auto serialized{verd.serialize()};
    EXPECT_STREQ(serialized.data(), "L_RSP_VERDICT 2 4 1");

    auto deserialized(deserialize(serialized));
    ASSERT_EQ( mtype_t::pm_lc_verdict_t, visit(caller_t{}, deserialized) );

    auto verd_deserialized{get<pm_lc_verdict_t>(deserialized) };
    EXPECT_EQ(verd_deserialized.fork_id, 2);
    EXPECT_EQ(verd_deserialized.n_prep_fork_id, 4);
    EXPECT_EQ(verd_deserialized.verdict, verdict_t::approved);
}
