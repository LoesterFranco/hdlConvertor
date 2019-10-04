#pragma once

#include <vector>

#include <hdlConvertor/svConvertor/sv2017Parser/sv2017Parser.h>
#include <hdlConvertor/hdlObjects/iHdlExpr.h>
#include <hdlConvertor/hdlObjects/hdlVariableDef.h>

#include <hdlConvertor/svConvertor/commentParser.h>

namespace hdlConvertor {
namespace sv {

class VerPortParser {
	SVCommentParser &commentParser;
public:
	using sv2017Parser = sv2017_antlr::sv2017Parser;
	using Non_ANSI_port_info_t = std::pair<std::string, std::vector<hdlObjects::HdlVariableDef*>>;
	std::vector<Non_ANSI_port_info_t> &non_ansi_port_groups;

	VerPortParser(SVCommentParser &commentParser,
			std::vector<Non_ANSI_port_info_t> &non_ansi_port_groups);
	std::vector<hdlObjects::HdlVariableDef*>* visitNonansi_port(
			sv2017Parser::Nonansi_portContext *ctx);
	std::vector<hdlObjects::HdlVariableDef*>* visitNonansi_port__expr(
			sv2017Parser::Nonansi_port__exprContext *ctx);
	std::vector<hdlObjects::HdlVariableDef*>* visitList_of_port_declarations(
			sv2017Parser::List_of_port_declarationsContext *ctx);
	/*
	 * @return <variable def, base type of variable>
	 * @note the ansi port can have chain of ports without direction and base type specified
	 * 		in this case type/dir of previous port should be used
	 * */
	std::pair<hdlObjects::HdlVariableDef*, hdlObjects::iHdlExpr*> visitAnsi_port_declaration(
			sv2017Parser::Ansi_port_declarationContext *ctx,
			hdlObjects::HdlVariableDef* prev_var,
			hdlObjects::iHdlExpr* prev_var_base_t);
	void visitNonansi_port_declaration(
			sv2017Parser::Nonansi_port_declarationContext *ctx,
			std::vector<hdlObjects::HdlVariableDef*> &res);
	void visitList_of_variable_port_identifiers(
			sv2017Parser::List_of_variable_port_identifiersContext *ctx,
			hdlObjects::iHdlExpr *base_type, bool latched,
			hdlObjects::HdlDirection direction, const std::string &doc,
			std::vector<hdlObjects::HdlVariableDef*> &res);
	void visitList_of_tf_variable_identifiers(
			sv2017Parser::List_of_tf_variable_identifiersContext *ctx,
			hdlObjects::iHdlExpr *base_type, bool latched,
			hdlObjects::HdlDirection direction, const std::string &doc,
			std::vector<hdlObjects::HdlVariableDef*> &res);
	/*
	 * @note the variables may contains also the specification of the size of the array from this reason
	 * 		the base_type does not have to be final type of the variable
	 * @note base_type is used in first variable, the rest will have independent copy of it
	 * */
	void visitList_of_variable_identifiers(
			sv2017Parser::List_of_variable_identifiersContext *ctx,
			hdlObjects::iHdlExpr *base_type, bool latched,
			hdlObjects::HdlDirection direction, const std::string &doc,
			std::vector<hdlObjects::HdlVariableDef*> &res);
	// wrap children on non_ansi ports to new ANSI port with name of parent port
	// module x (.a(b, c)); input b, c; endmodule -> module x (input [2:0] a); wire b, c; assign {b, c} = a; endmodule
	void convert_non_ansi_ports_to_ansi(
			std::vector<hdlObjects::HdlVariableDef*> &ports,
			std::vector<hdlObjects::iHdlObj*> &body);
	void visitTf_port_list(sv2017Parser::Tf_port_listContext *ctx,
			std::vector<hdlObjects::HdlVariableDef*> &res);
	hdlObjects::HdlVariableDef* visitTf_port_item(
			sv2017Parser::Tf_port_itemContext *ctx);
	hdlObjects::HdlDirection visitPort_direction(
			sv2017Parser::Port_directionContext *ctx);
	hdlObjects::HdlDirection visitTf_port_direction(
			sv2017Parser::Tf_port_directionContext *ctx);
};

}
}