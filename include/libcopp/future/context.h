#ifndef COPP_FUTURE_CONTEXT_H
#define COPP_FUTURE_CONTEXT_H

#pragma once

#include "poll.h"

namespace copp {
    namespace future {
        /**
         * @brief context_t
         * @note TPD::operator()(future_t<T>&, context_t<TPD>&) must be declared
         *         for private data type 'TPD' (template argument).
         *       TPD::operator()(future_t<T>&, context_t<TPD>&) will be called with private_data_(future, *this) when related event state
         *         changes. and the "future.poll_data()" must be set if all related asynchronous jobs is done.
         *
         */
        template <class TPD>
        class LIBCOPP_COPP_API_HEAD_ONLY context_t {
        public:
            typedef context_t<TPD>                   self_type;
            typedef std::function<void(self_type &)> wake_fn_t;
            typedef TPD                              value_type;

        private:
            // context can not be copy or moved.
            context_t(const context_t &) UTIL_CONFIG_DELETED_FUNCTION;
            context_t &operator=(const context_t &) UTIL_CONFIG_DELETED_FUNCTION;
            context_t(context_t &&) UTIL_CONFIG_DELETED_FUNCTION;
            context_t &operator=(context_t &&) UTIL_CONFIG_DELETED_FUNCTION;

        public:
            template <class... TARGS>
            context_t(TARGS &&... args) : private_data_(std::forward<TARGS>(args)...) {}

            template <class TFUTURE>
            void poll(TFUTURE &&fut) {
                private_data_(std::forward<TFUTURE>(fut), *this);
            }

            void wake() {
                if (wake_fn_) {
                    wake_fn_(*this);
                }
            }

            inline void             set_wake_fn(wake_fn_t fn) { wake_fn_ = fn; }
            inline const wake_fn_t &get_wake_fn() const { return wake_fn_; }
            inline wake_fn_t &      get_wake_fn() { return wake_fn_; }

            inline value_type &      get_private_data() UTIL_CONFIG_NOEXCEPT { return private_data_; }
            inline const value_type &get_private_data() const UTIL_CONFIG_NOEXCEPT { return private_data_; }

        private:
            value_type private_data_;
            wake_fn_t  wake_fn_;
        };

        /**
         * @brief context_t<void>
         * @note pool_fn(self_type&, poll_event_data_t) will be called with pool_fn(*this, {&out, private_data_}) when related event state
         *       changes, and the "out" must be set if the action is ready
         */
        template <>
        class LIBCOPP_COPP_API_HEAD_ONLY context_t<void> {
        public:
            struct poll_event_data_t {
                void *future_ptr;   // set to address of **fut** when call poll(future_t<T> &fut)
                void *private_data; // set to ptr passed by constructor
            };

            typedef context_t<void>                                     self_type;
            typedef std::function<void(self_type &)>                    wake_fn_t;
            typedef std::function<void(self_type &, poll_event_data_t)> poll_fn_t;
            typedef void *                                              value_type;

        private:
            // context can not be copy or moved.
            context_t(const context_t &) UTIL_CONFIG_DELETED_FUNCTION;
            context_t &operator=(const context_t &) UTIL_CONFIG_DELETED_FUNCTION;
            context_t(context_t &&) UTIL_CONFIG_DELETED_FUNCTION;
            context_t &operator=(context_t &&) UTIL_CONFIG_DELETED_FUNCTION;

            struct construct_helper_t {
                poll_fn_t pool_fn;
                void *    private_data;

                template <class U>
                inline construct_helper_t(U &&fn, void *ptr = NULL) : pool_fn(std::forward<U>(fn)), private_data(ptr) {}
            };

            template <bool>
            struct construct_helper_assign_t;

            template <>
            struct construct_helper_assign_t<true> {
                template <class U>
                static inline void assign(self_type &self, U &&helper) {
                    self.set_private_data(helper.private_data);
                    self.set_poll_fn(std::move(helper.pool_fn));
                }
            };

            template <>
            struct construct_helper_assign_t<false> {
                template <class U>
                static inline void assign(self_type &, U &&) {}
            };

        public:
            template <class U>
            static inline construct_helper_t construct(U &&fn, void *ptr = NULL) {
                return construct_helper_t(std::forward<U>(fn), ptr);
            }

            context_t() : private_data_(NULL) {}
            template <class TFIRST, class... TARGS>
            context_t(TFIRST &&helper, TARGS &&...) : private_data_(NULL) {
                construct_helper_assign_t<std::is_same<construct_helper_t, typename std::decay<TFIRST>::type>::value>::assign(
                    *this, std::forward<TFIRST>(helper));
            }

            ~context_t() {
                if (on_destroy_fn_) {
                    on_destroy_fn_(*this);
                }
            }

            template <class TFUTURE>
            void poll(TFUTURE &&fut) {
                if (poll_fn_) {
                    poll_event_data_t data;
                    data.future_ptr   = reinterpret_cast<void *>(&fut);
                    data.private_data = private_data_;
                    poll_fn_(*this, data);
                }
            }

            void wake() {
                if (wake_fn_) {
                    wake_fn_(*this);
                }
            }

            inline void             set_poll_fn(poll_fn_t fn) { poll_fn_.swap(fn); }
            inline const poll_fn_t &get_poll_fn() const { return poll_fn_; }
            inline poll_fn_t &      get_poll_fn() { return poll_fn_; }

            inline void             set_wake_fn(wake_fn_t fn) { wake_fn_.swap(fn); }
            inline const wake_fn_t &get_wake_fn() const { return wake_fn_; }
            inline wake_fn_t &      get_wake_fn() { return wake_fn_; }

            inline void             set_on_destroy(wake_fn_t fn) { on_destroy_fn_.swap(fn); }
            inline const wake_fn_t &get_on_destroy() const { return on_destroy_fn_; }
            inline wake_fn_t &      get_on_destroy() { return on_destroy_fn_; }

            inline void        set_private_data(void *ptr) { private_data_ = ptr; }
            inline void *      get_private_data() UTIL_CONFIG_NOEXCEPT { return private_data_; }
            inline const void *get_private_data() const UTIL_CONFIG_NOEXCEPT { return private_data_; }

        private:
            value_type private_data_;
            wake_fn_t  wake_fn_;
            poll_fn_t  poll_fn_;
            wake_fn_t  on_destroy_fn_;
        };
    } // namespace future
} // namespace copp

#endif