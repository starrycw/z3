/*++
Copyright (c) 2012 Microsoft Corporation

Module Name:

    api_array.cpp

Abstract:
    API for array theory

Author:

    Leonardo de Moura (leonardo) 2012-02-29.

Revision History:

--*/
#include "api/z3.h"
#include "api/api_log_macros.h"
#include "api/api_context.h"
#include "api/api_util.h"
#include "ast/array_decl_plugin.h"

extern "C" {

    Z3_sort Z3_API Z3_mk_array_sort(Z3_context c, Z3_sort domain, Z3_sort range) {
        Z3_TRY;
        LOG_Z3_mk_array_sort(c, domain, range);
        RESET_ERROR_CODE();
        parameter params[2]  = { parameter(to_sort(domain)), parameter(to_sort(range)) };
        sort * ty =  mk_c(c)->m().mk_sort(mk_c(c)->get_array_fid(), ARRAY_SORT, 2, params);
        mk_c(c)->save_ast_trail(ty);
        RETURN_Z3(of_sort(ty));
        Z3_CATCH_RETURN(nullptr);
    }

    Z3_sort Z3_API Z3_mk_array_sort_n(Z3_context c, unsigned n, Z3_sort const* domain, Z3_sort range) {
        Z3_TRY;
        LOG_Z3_mk_array_sort_n(c, n, domain, range);
        RESET_ERROR_CODE();
        vector<parameter> params;
        for (unsigned i = 0; i < n; ++i) params.push_back(parameter(to_sort(domain[i])));
        params.push_back(parameter(to_sort(range)));
        sort * ty =  mk_c(c)->m().mk_sort(mk_c(c)->get_array_fid(), ARRAY_SORT, params.size(), params.data());
        mk_c(c)->save_ast_trail(ty);
        RETURN_Z3(of_sort(ty));
        Z3_CATCH_RETURN(nullptr);
    }


    Z3_ast Z3_API Z3_mk_select(Z3_context c, Z3_ast a, Z3_ast i) {
        Z3_TRY;
        LOG_Z3_mk_select(c, a, i);
        RESET_ERROR_CODE();
        ast_manager & m = mk_c(c)->m();
        CHECK_IS_EXPR(a, nullptr);
        CHECK_IS_EXPR(i, nullptr);
        expr * _a        = to_expr(a);
        expr * _i        = to_expr(i);
        sort * a_ty = _a->get_sort();
        sort * i_ty = _i->get_sort();
        if (a_ty->get_family_id() != mk_c(c)->get_array_fid()) {
            SET_ERROR_CODE(Z3_SORT_ERROR, nullptr);
            RETURN_Z3(nullptr);
        }
        sort * domain[2] = {a_ty, i_ty};
        func_decl * d   = m.mk_func_decl(mk_c(c)->get_array_fid(), OP_SELECT, 2, a_ty->get_parameters(), 2, domain);
        expr * args[2]        = {_a, _i};
        app * r        = m.mk_app(d, 2, args);
        mk_c(c)->save_ast_trail(r);
        check_sorts(c, r);
        RETURN_Z3(of_ast(r));
        Z3_CATCH_RETURN(nullptr);
    }

    Z3_ast Z3_API Z3_mk_select_n(Z3_context c, Z3_ast a, unsigned n, Z3_ast const* idxs) {
        Z3_TRY;
        LOG_Z3_mk_select_n(c, a, n, idxs);
        RESET_ERROR_CODE();
        ast_manager & m = mk_c(c)->m();
        CHECK_IS_EXPR(a, nullptr);
        expr * _a        = to_expr(a);
        sort * a_ty = _a->get_sort();
        if (a_ty->get_family_id() != mk_c(c)->get_array_fid()) {
            SET_ERROR_CODE(Z3_SORT_ERROR, nullptr);
            RETURN_Z3(nullptr);
        }
        ptr_vector<sort> domain;
        ptr_vector<expr> args;
        args.push_back(_a);
        domain.push_back(a_ty);
        for (unsigned i = 0; i < n; ++i) {
            CHECK_IS_EXPR(idxs[i], nullptr);
            args.push_back(to_expr(idxs[i]));
            domain.push_back(to_expr(idxs[i])->get_sort());
        }
        func_decl * d   = m.mk_func_decl(mk_c(c)->get_array_fid(), OP_SELECT, 2, a_ty->get_parameters(), domain.size(), domain.data());
        app * r        = m.mk_app(d, args.size(), args.data());
        mk_c(c)->save_ast_trail(r);
        check_sorts(c, r);
        RETURN_Z3(of_ast(r));
        Z3_CATCH_RETURN(nullptr);
    }


    Z3_ast Z3_API Z3_mk_store(Z3_context c, Z3_ast a, Z3_ast i, Z3_ast v) {
        Z3_TRY;
        LOG_Z3_mk_store(c, a, i, v);
        RESET_ERROR_CODE();
        ast_manager & m = mk_c(c)->m();
        CHECK_IS_EXPR(a, nullptr);
        CHECK_IS_EXPR(i, nullptr);
        CHECK_IS_EXPR(v, nullptr);
        expr * _a        = to_expr(a);
        expr * _i        = to_expr(i);
        expr * _v        = to_expr(v);
        sort * a_ty = _a->get_sort();
        sort * i_ty = _i->get_sort();
        sort * v_ty = _v->get_sort();
        if (a_ty->get_family_id() != mk_c(c)->get_array_fid()) {
            SET_ERROR_CODE(Z3_SORT_ERROR, nullptr);
            RETURN_Z3(nullptr);
        }
        sort * domain[3] = {a_ty, i_ty, v_ty};
        func_decl * d   = m.mk_func_decl(mk_c(c)->get_array_fid(), OP_STORE, 2, a_ty->get_parameters(), 3, domain);
        expr * args[3]        = {_a, _i, _v};
        app * r        = m.mk_app(d, 3, args);
        mk_c(c)->save_ast_trail(r);
        check_sorts(c, r);
        RETURN_Z3(of_ast(r));
        Z3_CATCH_RETURN(nullptr);
    }

    Z3_ast Z3_API Z3_mk_store_n(Z3_context c, Z3_ast a, unsigned n, Z3_ast const* idxs, Z3_ast v) {
        Z3_TRY;
        LOG_Z3_mk_store_n(c, a, n, idxs, v);
        RESET_ERROR_CODE();
        ast_manager & m = mk_c(c)->m();
        expr * _a        = to_expr(a);
        expr * _v        = to_expr(v);
        sort * a_ty = _a->get_sort();
        sort * v_ty = _v->get_sort();
        if (a_ty->get_family_id() != mk_c(c)->get_array_fid()) {
            SET_ERROR_CODE(Z3_SORT_ERROR, nullptr);
            RETURN_Z3(nullptr);
        }
        ptr_vector<sort> domain;
        ptr_vector<expr> args;
        args.push_back(_a);
        domain.push_back(a_ty);
        for (unsigned i = 0; i < n; ++i) {
            args.push_back(to_expr(idxs[i]));
            domain.push_back(to_expr(idxs[i])->get_sort());
        }
        args.push_back(_v);
        domain.push_back(v_ty);
        func_decl * d   = m.mk_func_decl(mk_c(c)->get_array_fid(), OP_STORE, 2, a_ty->get_parameters(), domain.size(), domain.data());
        app * r        = m.mk_app(d, args.size(), args.data());
        mk_c(c)->save_ast_trail(r);
        check_sorts(c, r);
        RETURN_Z3(of_ast(r));
        Z3_CATCH_RETURN(nullptr);
    }


    Z3_ast Z3_API Z3_mk_map(Z3_context c, Z3_func_decl f, unsigned n, Z3_ast const* args) {
        Z3_TRY;
        LOG_Z3_mk_map(c, f, n, args);
        RESET_ERROR_CODE();
        if (n == 0) {
            SET_ERROR_CODE(Z3_INVALID_ARG, nullptr);
            RETURN_Z3(nullptr);
        }
        ast_manager & m = mk_c(c)->m();
        func_decl* _f      = to_func_decl(f);
        expr* const* _args = to_exprs(n, args);

        ptr_vector<sort> domain;
        for (unsigned i = 0; i < n; ++i) {
            domain.push_back(_args[i]->get_sort());
        }
        parameter param(_f);
        func_decl * d = m.mk_func_decl(mk_c(c)->get_array_fid(), OP_ARRAY_MAP, 1, &param, n, domain.data());
        app* r = m.mk_app(d, n, _args);
        mk_c(c)->save_ast_trail(r);
        check_sorts(c, r);
        RETURN_Z3(of_ast(r));
        Z3_CATCH_RETURN(nullptr);
    }

    Z3_ast Z3_API Z3_mk_const_array(Z3_context c, Z3_sort domain, Z3_ast v) {
        Z3_TRY;
        LOG_Z3_mk_const_array(c, domain, v);
        RESET_ERROR_CODE();
        ast_manager & m = mk_c(c)->m();
        expr * _v        = to_expr(v);
        sort * _range = _v->get_sort();
        sort * _domain = to_sort(domain);
        parameter params[2]  = { parameter(_domain), parameter(_range) };
        sort * a_ty    = mk_c(c)->m().mk_sort(mk_c(c)->get_array_fid(), ARRAY_SORT, 2, params);
        parameter param(a_ty);
        func_decl* cd   = m.mk_func_decl(mk_c(c)->get_array_fid(), OP_CONST_ARRAY, 1, &param, 1, &_range);
        app * r        = m.mk_app(cd, 1, &_v);
        mk_c(c)->save_ast_trail(r);
        check_sorts(c, r);
        RETURN_Z3(of_ast(r));
        Z3_CATCH_RETURN(nullptr);
    }

    Z3_ast Z3_API Z3_mk_array_default(Z3_context c, Z3_ast array) {
        Z3_TRY;
        LOG_Z3_mk_array_default(c, array);
        RESET_ERROR_CODE();
        ast_manager & m = mk_c(c)->m();
        expr * _a        = to_expr(array);

        func_decl * f   = m.mk_func_decl(mk_c(c)->get_array_fid(), OP_ARRAY_DEFAULT, 0, nullptr, 1, &_a);
        app * r        = m.mk_app(f, 1, &_a);
        mk_c(c)->save_ast_trail(r);
        check_sorts(c, r);
        RETURN_Z3(of_ast(r));
        Z3_CATCH_RETURN(nullptr);
    }

    Z3_ast mk_app_array_core(Z3_context c, Z3_sort domain, Z3_ast v) {
        RESET_ERROR_CODE();
        ast_manager & m = mk_c(c)->m();
        expr * _v        = to_expr(v);
        sort * _range = _v->get_sort();
        sort * _domain = to_sort(domain);
        parameter params[2]  = { parameter(_domain), parameter(_range) };
        sort * a_ty    = mk_c(c)->m().mk_sort(mk_c(c)->get_array_fid(), ARRAY_SORT, 2, params);
        parameter param(a_ty);
        func_decl * cd   = m.mk_func_decl(mk_c(c)->get_array_fid(), OP_CONST_ARRAY, 1, &param, 1, &_range);
        app * r        = m.mk_app(cd, 1, &_v);
        mk_c(c)->save_ast_trail(r);
        check_sorts(c, r);
        return of_ast(r);
    }

    Z3_sort Z3_API Z3_mk_set_sort(Z3_context c, Z3_sort ty) {
        Z3_TRY;
        return Z3_mk_array_sort(c, ty, Z3_mk_bool_sort(c));
        Z3_CATCH_RETURN(nullptr);
    }

    Z3_ast Z3_API Z3_mk_empty_set(Z3_context c, Z3_sort domain) {
        Z3_TRY;
        LOG_Z3_mk_empty_set(c, domain);
        RESET_ERROR_CODE();
        Z3_ast r = mk_app_array_core(c, domain, Z3_mk_false(c));
        RETURN_Z3(r);
        Z3_CATCH_RETURN(nullptr);
    }

    Z3_ast Z3_API Z3_mk_full_set(Z3_context c, Z3_sort domain) {
        Z3_TRY;
        LOG_Z3_mk_full_set(c, domain);
        RESET_ERROR_CODE();
        Z3_ast r = mk_app_array_core(c, domain, Z3_mk_true(c));
        RETURN_Z3(r);
        Z3_CATCH_RETURN(nullptr);
    }

    MK_NARY(Z3_mk_set_union, mk_c(c)->get_array_fid(), OP_SET_UNION, SKIP);
    MK_NARY(Z3_mk_set_intersect, mk_c(c)->get_array_fid(), OP_SET_INTERSECT, SKIP);
    MK_BINARY(Z3_mk_set_difference, mk_c(c)->get_array_fid(), OP_SET_DIFFERENCE, SKIP);
    MK_UNARY(Z3_mk_set_complement, mk_c(c)->get_array_fid(), OP_SET_COMPLEMENT, SKIP);
    MK_BINARY(Z3_mk_set_subset, mk_c(c)->get_array_fid(), OP_SET_SUBSET, SKIP);
    MK_BINARY(Z3_mk_array_ext, mk_c(c)->get_array_fid(), OP_ARRAY_EXT, SKIP);
    MK_BINARY(Z3_mk_set_has_size, mk_c(c)->get_array_fid(), OP_SET_HAS_SIZE, SKIP);

    Z3_ast Z3_API Z3_mk_as_array(Z3_context c, Z3_func_decl f) {
        Z3_TRY;
        LOG_Z3_mk_as_array(c, f);
        RESET_ERROR_CODE();
        ast_manager & m = mk_c(c)->m();
        array_util a(m);
        app * r = a.mk_as_array(to_func_decl(f));
        mk_c(c)->save_ast_trail(r);
        return of_ast(r);
        Z3_CATCH_RETURN(nullptr);
    }

    Z3_ast Z3_API Z3_mk_set_member(Z3_context c, Z3_ast elem, Z3_ast set) {
        return Z3_mk_select(c, set, elem);
    }

    Z3_ast Z3_API Z3_mk_set_add(Z3_context c, Z3_ast set, Z3_ast elem) {
        return Z3_mk_store(c, set, elem, Z3_mk_true(c));
    }

    Z3_ast Z3_API Z3_mk_set_del(Z3_context c, Z3_ast set, Z3_ast elem) {
        return Z3_mk_store(c, set, elem, Z3_mk_false(c));
    }

    static bool is_array_sort(Z3_context c, Z3_sort t) {
        return 
            to_sort(t)->get_family_id() == mk_c(c)->get_array_fid() &&
            to_sort(t)->get_decl_kind() == ARRAY_SORT;
    }

    unsigned Z3_API Z3_get_array_arity(Z3_context c, Z3_sort s) {
        Z3_TRY;
        LOG_Z3_get_array_arity(c, s);
        RESET_ERROR_CODE();
        sort * a = to_sort(s);
        if (Z3_get_sort_kind(c, s) != Z3_ARRAY_SORT) {
            SET_ERROR_CODE(Z3_INVALID_ARG, "sort should be an array");
            return 0;
        }
        return get_array_arity(a);
        Z3_CATCH_RETURN(0);
    }

    Z3_sort Z3_API Z3_get_array_sort_domain(Z3_context c, Z3_sort t) {
        Z3_TRY;
        LOG_Z3_get_array_sort_domain(c, t);
        RESET_ERROR_CODE();
        CHECK_VALID_AST(t, nullptr);
        if (is_array_sort(c, t)) {
            Z3_sort r = of_sort(get_array_domain(to_sort(t), 0));
            RETURN_Z3(r);
        }
        SET_ERROR_CODE(Z3_INVALID_ARG, nullptr);
        RETURN_Z3(nullptr);
        Z3_CATCH_RETURN(nullptr);
    }

    Z3_sort Z3_API Z3_get_array_sort_domain_n(Z3_context c, Z3_sort t, unsigned idx) {
        Z3_TRY;
        LOG_Z3_get_array_sort_domain_n(c, t, idx);
        RESET_ERROR_CODE();
        CHECK_VALID_AST(t, nullptr);
        if (is_array_sort(c, t) && get_array_arity(to_sort(t)) > idx) {
            Z3_sort r = of_sort(get_array_domain(to_sort(t), idx));
            RETURN_Z3(r);
        }
        SET_ERROR_CODE(Z3_INVALID_ARG, nullptr);
        RETURN_Z3(nullptr);
        Z3_CATCH_RETURN(nullptr);
    }

    Z3_sort Z3_API Z3_get_array_sort_range(Z3_context c, Z3_sort t) {
        Z3_TRY;
        LOG_Z3_get_array_sort_range(c, t);
        RESET_ERROR_CODE();
        CHECK_VALID_AST(t, nullptr);
        if (is_array_sort(c, t)) {
            Z3_sort r = of_sort(get_array_range(to_sort(t)));
            RETURN_Z3(r);
        }
        SET_ERROR_CODE(Z3_INVALID_ARG, nullptr);
        RETURN_Z3(nullptr);
        Z3_CATCH_RETURN(nullptr);
    }

};
