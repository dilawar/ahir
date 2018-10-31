//
// Copyright (C) 2010-: Madhav P. Desai
// All Rights Reserved.
//  
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal with the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimers.
//  * Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimers in the documentation and/or other materials provided
//    with the distribution.
//  * Neither the names of the AHIR Team, the Indian Institute of
//    Technology Bombay, nor the names of its contributors may be used
//    to endorse or promote products derived from this Software
//    without specific prior written permission.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.
#include <vcIncludes.hpp>
#include <vcRoot.hpp>
#include <vcType.hpp>
#include <vcValue.hpp>
#include <vcControlPath.hpp>
#include <vcOperator.hpp>
#include <vcMemorySpace.hpp>
#include <vcDataPath.hpp>
#include <vcModule.hpp>
#include <vcSystem.hpp>

void vcDatapathElement::Print_VHDL_Level_Logger(vcModule* m, ostream& ofile)
{
	// TODO
}

void vcDatapathElement::Print_VHDL_Level_Protocol_Signals(vcWire* w, ostream& ofile)
{
	// TODO.
}

void vcDatapathElement::Print_VHDL_DPE_Level_Protocol_Signals(ostream& ofile)
{
	// TODO.
}

void vcDataPath::Print_VHDL_Level(ostream& ofile)
{
	ofile << "data_path: Block -- { " << endl;

	// print wires.
	for(map<string, vcWire*>::iterator iter = _wire_map.begin();
			iter != _wire_map.end();
			iter++)
	{
		(*iter).second->Print_VHDL_Std_Logic_Declaration(ofile);
		this->Print_VHDL_Level_Protocol_Signals((*iter).second, ofile);
	}

	this->Get_Parent()->Print_VHDL_Pipe_Signals(ofile);


	// for each DPE, print a pair of sample req/ack  level signals
	//  and a pair of update req/ack level signals.
	this->Print_VHDL_DPE_Level_Protocol_Signals(ofile);


	ofile << "-- }" << endl << "begin -- { " << endl;


	// tie constant wires to their values.
	for(map<string, vcWire*>::iterator iter = _wire_map.begin();
			iter != _wire_map.end();
			iter++)
	{
		if((*iter).second->Is("vcConstantWire"))
			ofile << ((*iter).second)->Get_VHDL_Signal_Id() << " <= " << ((vcConstantWire*)((*iter).second))->Get_Value()->To_VHDL_String() << ";" << endl;
		else
			this->Print_VHDL_Level_Forks((*iter).second, ofile);
	}

	// now instantiate each group. 
	this->Print_VHDL_Select_Instances_Level(ofile); 
	this->Print_VHDL_Slice_Instances_Level(ofile); 
	this->Print_VHDL_Permutation_Instances_Level(ofile);
	this->Print_VHDL_Register_Instances_Level(ofile);
	this->Print_VHDL_Interlock_Buffer_Instances_Level(ofile); 
	this->Print_VHDL_Equivalence_Instances_Level(ofile);
	this->Print_VHDL_Split_Operator_Instances(ofile);
	this->Print_VHDL_Load_Instances(ofile); 
	this->Print_VHDL_Store_Instances(ofile);
	this->Get_Parent()->Print_VHDL_Pipe_Instances(ofile);
	this->Print_VHDL_Inport_Instances_Level(ofile);
	this->Print_VHDL_Outport_Instances_Level(ofile);
	this->Print_VHDL_Call_Instances_Level(ofile);


	ofile << "-- }" << endl << "end Block; -- data_path" << endl;
}



void vcDataPath::Print_VHDL_Select_Instances_Level(ostream& ofile)
{ 
	for(map<string, vcSelect*>::iterator iter = _select_map.begin();
			iter != _select_map.end();
			iter++)
	{
		vcSelect* s = (*iter).second;
		if(vcSystem::_enable_logging)
			s->vcSplitOperator::Print_VHDL_Logger_Level(this->Get_Parent(), ofile);
		}
		s->Print_VHDL_Level(ofile);
	}
}


void vcDataPath::Print_VHDL_Slice_Instances_Level(ostream& ofile)
{ 
	for(map<string, vcSlice*>::iterator iter = _slice_map.begin();
			iter != _slice_map.end();
			iter++)
	{
		vcSlice* s = (*iter).second;
		if(vcSystem::_enable_logging)
			s->vcSplitOperator::Print_VHDL_Logger_Level(this->Get_Parent(), ofile);

		s->Print_VHDL_Level(ofile);
	}
}

void vcDataPath::Print_VHDL_Permutation_Instances_Level(ostream& ofile)
{ 
	for(map<string, vcPermutation*>::iterator iter = _permutation_map.begin();
			iter != _permutation_map.end();
			iter++)
	{
		vcPermutation* s = (*iter).second;
		if(vcSystem::_enable_logging)
			s->vcSplitOperator::Print_VHDL_Logger_Level(this->Get_Parent(), ofile);

		s->Print_VHDL_Level(ofile);
	}
}

void vcDataPath::Print_VHDL_Register_Instances_Level(ostream& ofile)
{ 

	for(map<string, vcRegister*>::iterator iter = _register_map.begin();
			iter != _register_map.end();
			iter++)
	{
		vcRegister* s = (*iter).second;
		if(vcSystem::_enable_logging)
			s->vcOperator::Print_VHDL_Logger_Level(this->Get_Parent(), ofile);
		s->Print_VHDL_Level(ofile);
	}
}

void vcDataPath::Print_VHDL_Interlock_Buffer_Instances_Level(ostream& ofile)
{
	string parent_name = this->Get_Parent()->Get_Id();
	for(map<string, vcInterlockBuffer*>::iterator iter = _interlock_buffer_map.begin();
			iter != _interlock_buffer_map.end();
			iter++)
	{
		vcInterlockBuffer* p = (*iter).second;
		if(vcSystem::_enable_logging)
		{
			p->vcSplitOperator::Print_VHDL_Logger_Level(this->Get_Parent(), ofile);
		}
		p->Print_VHDL_Level(ofile);
	}
}


void vcDataPath::Print_VHDL_Equivalence_Instances_Level(ostream& ofile)
{ 
	for(map<string, vcEquivalence*>::iterator iter = _equivalence_map.begin();
			iter != _equivalence_map.end();
			iter++)
	{
		vcEquivalence* s = (*iter).second;
		if(vcSystem::_enable_logging)
			s->vcOperator::Print_VHDL_Logger_Level(this->Get_Parent(), ofile);

		s->Print_VHDL_Level(ofile);
	}
}

//
// A hell of an ugly function.. Can it be improved?
// Basically, look at each operator group,
// collect the information for the inputs and outputs
// and loads of other stuff.  There are three distinct
// case:
//    1:  Pipelined FP operator (shared)
//    2:  Unshared operator (not pipelined FP)
//    3.  Shared operator (not pipelined FP)
//
// Break it up into three functions depending on the case?
// We just need to add input-buffering to it for now..
// Not worth touching at this point.
//
//
// added a bypass to handle the flow-through case separately.
//
void vcDataPath::Print_VHDL_Split_Operator_Instances_Level(ostream& ofile)
{
	string group_name;
	string no_arb_string; 
	string parent_name = this->Get_Parent()->Get_Id();

	for(int idx = 0, fidx = this->_compatible_split_operator_groups.size(); idx < fidx; idx++)
	{ // for each operator group.

		bool is_unary_operator = false;
		// number of requesters.
		int num_reqs = _compatible_split_operator_groups[idx].size();
		vcSplitOperator* lead_op = ((vcSplitOperator*)(*(_compatible_split_operator_groups[idx].begin())));

		// used in the unshared case.
		bool flow_through = (lead_op->Get_Flow_Through() || this->Get_Parent()->Get_Volatile_Flag());
		bool full_rate = lead_op->Get_Full_Rate();

		//
		// if it is a flow-through operator, just print 
		// it as a concurrent statement.
		// 
		if(flow_through && (num_reqs == 1))
		{
			if(vcSystem::_enable_logging)
				lead_op->vcSplitOperator::Print_VHDL_Logger_Level(this->Get_Parent(), ofile);
			lead_op->Print_VHDL_Level(ofile);
			continue;
		}

		// to collect inwires, outwires and reqs/acks.
		vector<vcWire*> inwires;
		vector<int> inwire_buffering;

		vector<string> reqL;
		vector<string> ackL;
		vector<string> reqR;
		vector<string> ackR;

		vector<vcWire*> outwires;
		vector<int> outwire_buffering;

		vector<vcWire*> guard_wires;
		vector<bool> guard_complements;



		// to get the operation id, we need the vc operator as well as the input and output 
		// types. (e.g. + (float float) (float) or + (int int) (int) ?
		vcType* input_type =   lead_op->Get_Input_Type();
		vcType* output_type =   lead_op->Get_Output_Type();
		string vc_op_id = lead_op->Get_Op_Id();
		string vhdl_op_id = Get_VHDL_Op_Id(vc_op_id,
				input_type,
				output_type, true);

		string s__id = StripBracketingQuotes(vhdl_op_id);
		group_name = s__id + "_group_" + IntToStr(idx) ;

		bool is_pipelined_op = lead_op->Is_Pipelined_Operator();
		bool is_float_to_float = lead_op->Is_Float_To_Float_Operator();

		// for pipelined ops.
		if(is_pipelined_op)
		{
			no_arb_string = "false";
		}
		else
		{
			no_arb_string = (vcSystem::_min_area_flag ? "false" : "true");
		}

		// the number of inputs and outputs on each operator in the current group.
		int num_ips = ((vcSplitOperator*)(*(_compatible_split_operator_groups[idx].begin())))->Get_Number_Of_Input_Wires();
		int num_ops = ((vcSplitOperator*)(*(_compatible_split_operator_groups[idx].begin())))->Get_Number_Of_Output_Wires();

		// to keep track of the ids of the operators in this group.
		vector<string> elements;

		// to keep track of the operators in this group.
		vector<vcDatapathElement*> dpe_elements;


		// ok. collect all the information..
		for(set<vcDatapathElement*>::iterator iter = _compatible_split_operator_groups[idx].begin();
				iter != _compatible_split_operator_groups[idx].end();
				iter++)
		{

			vcSplitOperator* so = (vcSplitOperator*) (*iter);
			if(vcSystem::_enable_logging)
				so->vcSplitOperator::Print_VHDL_Logger(this->Get_Parent(), ofile);
			is_unary_operator = Is_Unary_Op(so->Get_Op_Id());

			dpe_elements.push_back(so);
			elements.push_back(so->Get_VHDL_Id());

			so->Append_Inwires(inwires);
			so->Append_Inwire_Buffering(inwire_buffering);

			so->Append_Outwires(outwires);
			so->Append_Outwire_Buffering(outwire_buffering, num_reqs);

			reqL.push_back(so->Get_Req_Level(0));
			ackL.push_back(so->Get_Ack_Level(0));
			reqR.push_back(so->Get_Req_Level(1));
			ackR.push_back(so->Get_Ack_Level(1));

			so->Append_Guard(guard_wires,guard_complements);
		}


		string input_buffering_string;
		int max_inbuf = this->Generate_Buffering_String("inBUFs", inwire_buffering, input_buffering_string);

		string output_buffering_string;
		int max_outbuf = this->Generate_Buffering_String("outBUFs", outwire_buffering, output_buffering_string);

		string guard_flags;
		string guard_buffering;
		Generate_Guard_Constants(guard_buffering, guard_flags, dpe_elements, guard_wires);

		// is the second input a constant?
		bool use_constant = false;
		string const_operand;
		int const_width = 1;
		if((!is_pipelined_op) && (num_ips == 2 && inwires[1]->Is("vcConstantWire")))
		{
			num_ips = 1; // has only one input, we will be using one constant operand.
			use_constant = true;
			const_operand = ((vcConstantWire*)inwires[1])->Get_Value()->To_VHDL_String();
			const_width = ((vcConstantWire*)inwires[1])->Get_Size();
		}
		else
		{
			const_operand += "\"0\"";
		}


		// total in-width 
		int in_width = 0;
		vector<vcWire*> concat_in_wires;
		for(int u = 0; u < inwires.size(); u++)
		{
			if(!use_constant || (u%2 == 0))
			{
				// do not count if inwire is
				// an even operand and if it is
				// a constant.
				in_width += inwires[u]->Get_Size();
				concat_in_wires.push_back(inwires[u]);
			}
		}


		// total out-width..
		int out_width = 0;
		for(int u = 0; u < outwires.size(); u++)
		{
			out_width += outwires[u]->Get_Size();
		}

		// VHDL code for this shared group
		ofile << "-- shared split operator group (" << idx << ") : " ;
		for(int u = 0; u < elements.size(); u++)
			ofile << elements[u] << " ";
		ofile << endl;

		bool use_regulator = false;
		if((num_reqs > 1) || is_pipelined_op)
		{
			use_regulator = true;
		}

		// make a block
		ofile <<  group_name << ": Block -- {" << endl;
		// in and out data.
		ofile << "signal data_in: std_logic_vector(" << in_width-1 << " downto 0);" << endl;
		ofile << "signal data_out: std_logic_vector(" << out_width-1 << " downto 0);" << endl;
		// in and out acks.
		ofile << "signal reqR, ackR, reqL, ackL : std_logic_vector( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal reqR_unguarded, ackR_unguarded, reqL_unguarded, ackL_unguarded : std_logic_vector( " << num_reqs-1 << " downto 0);" << endl;
		if(use_regulator)
			ofile << "signal reqL_unregulated, ackL_unregulated : std_logic_vector( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal guard_vector : std_logic_vector( " << num_reqs-1 << " downto 0);" << endl;

		ofile << input_buffering_string << endl;
		ofile << output_buffering_string << endl;
		ofile << guard_flags << endl;
		ofile << guard_buffering << endl;


		ofile << "-- }\n begin -- {" << endl;

		// concatenate data_in
		Print_VHDL_Concatenation(string("data_in"), concat_in_wires,ofile);

		// disconcatenate data_out
		Print_VHDL_Disconcatenation(string("data_out"), out_width, outwires,ofile);

		// prepare guard vector.
		Print_VHDL_Guard_Concatenation(num_reqs, "guard_vector", guard_wires, guard_complements, ofile);

		// input types 
		vcType* input_type_1 = inwires[0]->Get_Type();
		vcType* input_type_2 = input_type_1;

		if(num_ips == 2)
			input_type_2 = inwires[1]->Get_Type();


		// the guards and the regulators..
		if((num_reqs > 1) || is_pipelined_op)
		{
			Print_VHDL_Concatenate_Req("reqL_unguarded",reqL,ofile);
			Print_VHDL_Disconcatenate_Ack("ackL_unguarded",ackL,ofile);
			Print_VHDL_Concatenate_Req("reqR_unguarded",reqR,ofile);
			Print_VHDL_Disconcatenate_Ack("ackR_unguarded",ackR,ofile);

			this->Print_VHDL_Regulator_Instance_Level(group_name + "_accessRegulator", num_reqs,"reqL_unregulated", "ackL_unregulated", "reqL", "ackL", "reqR", "ackR", dpe_elements, ofile);
			this->Print_VHDL_Guard_Instance_Level(false, false, group_name + "_gI",num_reqs,"guardBuffering","guardFlags","guard_vector",
					"reqL_unguarded", "ackL_unguarded",
					"reqL_unregulated", "ackL_unregulated",
					"reqR_unguarded", "ackR_unguarded",
					"reqR", "ackR", ofile);

		}
		else
		{
			// an unshared operator.
			Print_VHDL_Concatenate_Req("reqL_unguarded",reqL,ofile);
			Print_VHDL_Disconcatenate_Ack("ackL_unguarded",ackL,ofile);
			Print_VHDL_Concatenate_Req("reqR_unguarded",reqR,ofile);
			Print_VHDL_Disconcatenate_Ack("ackR_unguarded",ackR,ofile);

			if(!flow_through)
			{
				Print_VHDL_Guard_Instance_Level(false,false,group_name + "_gI",num_reqs,"guardBuffering","guardFlags","guard_vector",
						"reqL_unguarded", "ackL_unguarded",
						"reqL", "ackL",
						"reqR_unguarded", "ackR_unguarded",
						"reqR", "ackR", ofile);
			}
			else
			{
				ofile << "reqL <= reqL_unguarded;" << endl;
				ofile << "reqR <= reqR_unguarded;" << endl;
				ofile << "ackL_unguarded <= ackL;" << endl;
				ofile << "ackR_unguarded <= ackR;" << endl;
			}

		}


		// now the operator instance.
		// it can be an instance either of 
		//   1. a shared operator. 
		//   2. an unshared operator.
		//   3. a pipelined FP (ieee754) operator.
		// 
		// This stuff can be simplified considerably by just 
		// moving all the complexity to the VHDL library.
		// This is probably the right solution, because it decouples
		// vc2vhdl from the VHDL library.
		if(is_pipelined_op)
		{
			// for the moment.
			assert(input_type == output_type);
			assert(input_type->Is_Float_Type() && output_type->Is_Float_Type());

			int exp_width =  ((vcFloatType*) input_type)->Get_Characteristic_Width();
			int frac_width =  ((vcFloatType*) input_type)->Get_Mantissa_Width();

			ofile << "PipedFpOp: PipelinedFPOperatorLevel -- {" << endl;
			ofile << " generic map( -- { " << endl 
				<< " name => " << '"' << group_name << '"' << "," << endl 
				<< " operator_id => " << vhdl_op_id << "," << endl
				<< " exponent_width => " << exp_width << "," << endl
				<< " fraction_width => " << frac_width << ", " << endl
				<< " no_arbitration => " << no_arb_string << ","  << endl
				<< " num_reqs => " << num_reqs << "," << endl
				<< " use_input_buffering => true," << endl
				<< " detailed_buffering_per_input => inBUFs," << endl
				<< " detailed_buffering_per_output => outBUFs -- } \n )" << endl;
			ofile << "port map ( reqL => reqL , ackL => ackL, reqR => reqR, ackR => ackR, dataL => data_in, dataR => data_out, clk => clk, reset => reset); -- }" << endl;
			ofile << "-- } \n end Block; -- split operator group " << idx << endl; // thats it..
		}
		else
		{
			if((num_reqs > 1) || is_float_to_float)
			{
				// ok, a shared operator
				//   note: float-to-float will be unshared, but we still stuff
				//         it in here.  There is no extra cost.

				ofile << "SplitOperator: SplitOperatorSharedLevel -- {" << endl;
				ofile << "generic map ( -- { " ;
				ofile << " name => " << '"' << group_name << '"' << "," << endl;
				// a ton of generics..
				ofile << " operator_id => " << vhdl_op_id << "," << endl  // operator-id
					<< " input1_is_int => " << (input_type_1->Is_Int_Type() ? "true" : "false") << ", " << endl  // first op is int?
					<< " input1_characteristic_width => " 
					<< (input_type_1->Is("vcFloatType") ? ((vcFloatType*)input_type_1)->Get_Characteristic_Width() : 0) << ", " << endl // char 1
					<< " input1_mantissa_width    => " 
					<< (input_type_1->Is("vcFloatType") ? ((vcFloatType*)input_type_1)->Get_Mantissa_Width() : 0) << ", " << endl // mantissa 1
		    << " iwidth_1  => " << input_type_1->Size() << "," << endl // width 1
		    << " input2_is_int => " << (input_type_2->Is_Int_Type() ? "true" : "false") << ", " << endl  // second op is int?
		    << " input2_characteristic_width => " 
		    << (input_type_2->Is("vcFloatType") ? ((vcFloatType*)input_type_2)->Get_Characteristic_Width() : 0) << ", " << endl // char 2
		    << " input2_mantissa_width => "
		    << (input_type_2->Is("vcFloatType") ? ((vcFloatType*)input_type_2)->Get_Mantissa_Width() : 0) << ", " << endl // mantissa 2
		    << " iwidth_2      => " << (num_ips == 2 ? input_type_2->Size() : 0) << ", " << endl // width 2
		    << " num_inputs    => " << num_ips << ","  << endl // number of inputs
		    << " output_is_int => " << (output_type->Is_Int_Type() ? "true" : "false") << "," << endl // output is int?
		    << " output_characteristic_width  => " 
		    << (output_type->Is("vcFloatType") ? ((vcFloatType*)output_type)->Get_Characteristic_Width() : 0) << ", " << endl // char op
		    << " output_mantissa_width => " 
		    << (output_type->Is("vcFloatType") ? ((vcFloatType*)output_type)->Get_Mantissa_Width() : 0) << ", " << endl // mantissa op
		    << " owidth => " << output_type->Size() << "," << endl  // output width
		    << " constant_operand => " << const_operand << "," << endl // constant operand?
		    << " constant_width => " << const_width << "," << endl // constant width
		    << " use_constant  => " << (use_constant ? "true" : "false") << "," << endl // use constant?
		    << " full_rate  => " << (full_rate ? "true" : "false") << "," << endl // full-rate?
		    << " no_arbitration => " << no_arb_string << "," << endl
		    << " min_clock_period => " << (vcSystem::_min_clock_period_flag ? "true" : "false") << "," << endl
		    << " num_reqs => " << num_reqs << "," << endl
		    << " use_input_buffering => true," << endl
		    << " detailed_buffering_per_input => inBUFs," << endl
		    << " detailed_buffering_per_output => outBUFs --} \n )" << endl; // number of requesters..
	      ofile << "port map ( reqL => reqL , ackL => ackL, reqR => reqR, ackR => ackR, dataL => data_in, dataR => data_out, clk => clk, reset => reset); -- }" << endl;
	    }
	  else
	    {
	      string iname = '"' + group_name + '"';
	      int bufv = ( (max_inbuf > max_outbuf) ? max_inbuf : max_outbuf);
	      ofile << "UnsharedOperator: UnsharedOperatorWithBufferingLevel -- {" << endl;
	      ofile << "generic map ( -- { " ;
	      // a ton of generics..
	      ofile << " operator_id => " << vhdl_op_id << "," << endl  // operator-id
		    << " name => " << iname << "," << endl
		    << " input1_is_int => " << (input_type_1->Is_Int_Type() ? "true" : "false") << ", " << endl  // first op is int?
		    << " input1_characteristic_width => " 
		    << (input_type_1->Is("vcFloatType") ? ((vcFloatType*)input_type_1)->Get_Characteristic_Width() : 0) << ", " << endl // char 1
		    << " input1_mantissa_width    => " 
		    << (input_type_1->Is("vcFloatType") ? ((vcFloatType*)input_type_1)->Get_Mantissa_Width() : 0) << ", " << endl // mantissa 1
		    << " iwidth_1  => " << input_type_1->Size() << "," << endl // width 1
		    << " input2_is_int => " << (input_type_2->Is_Int_Type() ? "true" : "false") << ", " << endl  // second op is int?
		    << " input2_characteristic_width => " 
		    << (input_type_2->Is("vcFloatType") ? ((vcFloatType*)input_type_2)->Get_Characteristic_Width() : 0) << ", " << endl // char 2
		    << " input2_mantissa_width => "
		    << (input_type_2->Is("vcFloatType") ? ((vcFloatType*)input_type_2)->Get_Mantissa_Width() : 0) << ", " << endl // mantissa 2
		    << " iwidth_2      => " << (num_ips == 2 ? input_type_2->Size() : 0) << ", " << endl // width 2
		    << " num_inputs    => " << num_ips << ","  << endl // number of inputs
		    << " output_is_int => " << (output_type->Is_Int_Type() ? "true" : "false") << "," << endl // output is int?
		    << " output_characteristic_width  => " 
		    << (output_type->Is("vcFloatType") ? ((vcFloatType*)output_type)->Get_Characteristic_Width() : 0) << ", " << endl // char op
		    << " output_mantissa_width => " 
		    << (output_type->Is("vcFloatType") ? ((vcFloatType*)output_type)->Get_Mantissa_Width() : 0) << ", " << endl // mantissa op
		    << " owidth => " << output_type->Size() << "," << endl  // output width
		    << " constant_operand => " << const_operand << "," << endl // constant operand?
		    << " constant_width => " << const_width << "," << endl // constant width
		    << " buffering  => " << bufv << "," << endl // buffering in operator.
		    << " flow_through => " << (flow_through ? "true" : "false") << "," << endl
		    << " full_rate  => " << (full_rate ? "true" : "false") << "," << endl // full-rate?
		    << " use_constant  => " << (use_constant ? "true" : "false") << endl // use constant?

		    << "--} \n ) " << endl; // number of requesters..
	      ofile << "port map ( -- { " << endl
		    << "reqL => reqL(0)," << endl
		    << "ackL => ackL(0)," << endl
		    << "reqR => reqR(0)," << endl
		    << "ackR => ackR(0)," << endl
		    << "dataL => data_in, " << endl
		    << "dataR => data_out," << endl
		    << "clk => clk," << endl
		    << "reset => reset); -- }}" << endl;
	    }
	  ofile << "-- } \n end Block; -- split operator group " << idx << endl; // thats it..
	}
    }
}



void vcDataPath::Print_VHDL_Regulator_Instance_Level(string inst_id, 
			int num_reqs,  
			string reqs, string acks,
			string regulated_reqs, string regulated_acks, 
			string release_reqs, string release_acks,
			vector<vcDatapathElement*>& dpe_elements,
			ostream& ofile)
{


	// redundancy!
	assert(dpe_elements.size() == num_reqs);

	if(num_reqs == 1)
	{
		ofile << regulated_reqs << " <= " << reqs << ";" << endl;
		ofile << acks << " <= " << regulated_acks << ";" << endl;
		return;
	}

	int idx;
        string inst_id_idx;
	for(idx = 0; idx < num_reqs; idx++)
	{
		inst_id_idx = inst_id + "_" + IntToStr(idx);

		vcDatapathElement* dpe = dpe_elements[(num_reqs-idx)-1];
		
		// needs to get more sophisticated...
		// right now, the same buffering is used for
		// operations in a pipelined loop.
		int depth, num_slots;
		bool cf = dpe->Is_Part_Of_Pipelined_Loop(depth, num_slots);

		ofile << inst_id_idx 
			<< ": access_regulator_base_level generic map ("
			<< "name => " 
			<< '"' << inst_id_idx  << '"'
			<< ", num_slots => " 
			<< IntToStr(num_slots) <<") -- {" << endl;
		ofile << "port map (req => " << reqs << "(" << idx << "), -- {" << endl; 
		ofile << "ack => " << acks << "(" << idx << ")," << endl;
		ofile << "regulated_req => " << regulated_reqs << "(" << idx << ")," << endl;
		ofile << "regulated_ack => " << regulated_acks << "(" << idx << ")," << endl;
		ofile << "release_req => " << release_reqs << "(" << idx << ")," << endl;
		ofile << "release_ack => " << release_acks << "(" << idx << ")," << endl;
		ofile << "clk => clk, reset => reset); -- }}" << endl;
	}
}

void vcDataPath::Print_VHDL_Load_Instances_Level(ostream& ofile)
{ 

  string no_arb_string = (vcSystem::_min_area_flag ? "false" : "true");
  string parent_name = this->Get_Parent()->Get_Id();
  if(this->Get_Parent()->Get_Volatile_Flag() && (this->_compatible_load_groups.size() > 0))
  {
	vcSystem::Error("volatile module " + this->Get_Parent()->Get_VHDL_Id() + " cannot have loads.");
	return;
  }

  // print LoadReqShared instance and LoadCompleteShared instance.
  for(int idx = 0; idx < this->_compatible_load_groups.size(); idx++)
    { // for each operator group.

      // number of requesters.
      int num_reqs = _compatible_load_groups[idx].size();

      // to collect inwires, outwires and reqs/acks.
      vector<vcWire*> inwires;
      vector<int>  inwire_buffering;

      vector<vcWire*> guard_wires;
      vector<bool> guard_complements;

      vector<string> reqL;
      vector<string> ackL;
      vector<string> reqR;
      vector<string> ackR;

      vector<vcWire*> outwires;
      vector<int> outwire_buffering;

      // to keep track of the ids of the operators in this group.
      vector<string> elements;
      vector<vcDatapathElement*> dpe_elements;

      vcMemorySpace* ms = NULL;

      // ok. collect all the information..
      for(set<vcDatapathElement*>::iterator iter = _compatible_load_groups[idx].begin();
	  iter != _compatible_load_groups[idx].end();
	  iter++)
	{

	  assert((*iter)->Is("vcLoad"));
	  vcLoad* so = (vcLoad*) (*iter);
	  if(vcSystem::_enable_logging)
	  	so->vcSplitOperator::Print_VHDL_Logger_Level(this->Get_Parent(), ofile);

	  if(ms == NULL)
	    ms = ((vcLoad*) so)->Get_Memory_Space();
	  else
	    assert(ms == ((vcLoad*) so)->Get_Memory_Space());

	  elements.push_back(so->Get_VHDL_Id());
	  dpe_elements.push_back(so);

	  so->Append_Inwires(inwires);
	  so->Append_Inwire_Buffering(inwire_buffering);

	  so->Append_Outwires(outwires);
	  so->Append_Outwire_Buffering(outwire_buffering, num_reqs);
   
	  reqL.push_back(so->Get_Req_Level(0));
	  ackL.push_back(so->Get_Ack_Level(0));
	  reqR.push_back(so->Get_Req_Level(1));
	  ackR.push_back(so->Get_Ack_Level(1));

	  so->Append_Guard(guard_wires,guard_complements);
	}

      assert(ms != NULL);

      // address, data
      int addr_width = ((vcLoadStore*)(*_compatible_load_groups[idx].begin()))->Get_Address()->Get_Size();
      int data_width = ((vcLoadStore*)(*_compatible_load_groups[idx].begin()))->Get_Data()->Get_Size();
      
      // tag-length
      int tag_length = ms->Get_Tag_Length();

      // total in-width 
      int in_width = 0;
      for(int u = 0; u < inwires.size(); u++)
	{
	  in_width += inwires[u]->Get_Size();
	}

      // total out-width..
      int out_width = 0;
      for(int u = 0; u < outwires.size(); u++)
	{
	  out_width += outwires[u]->Get_Size();
	}

      // pipelined case: slots per output at the operator.
      vector<int> slot_demands;
      int mbuf_size = this->Generate_Pipeline_Slot_Demands(dpe_elements, slot_demands);

      // VHDL code for this shared group
      ofile << "-- shared load operator group (" << idx << ") : " ;
      for(int u = 0; u < elements.size(); u++)
	ofile << elements[u] << " ";
      ofile << endl;

      // string buffering_string;
      // this->Generate_Buffering_Constant_Declaration(dpe_elements, buffering_string);

      string input_buffering_string;
      int maxipb = this->Generate_Buffering_String("inBUFs", inwire_buffering, input_buffering_string);

      string output_buffering_string;
      int maxopb = this->Generate_Buffering_String("outBUFs", outwire_buffering, output_buffering_string);

      string guard_flags;
      string guard_buffering;
      Generate_Guard_Constants(guard_buffering, guard_flags, dpe_elements, guard_wires);

      string group_name = "LoadGroup" + IntToStr(idx);
      // make a block
      ofile << group_name << ": Block -- {" << endl;
      // in and out data.
      ofile << "signal data_in: std_logic_vector(" << in_width-1 << " downto 0);" << endl;
      ofile << "signal data_out: std_logic_vector(" << out_width-1 << " downto 0);" << endl;
      // in and out acks.
      ofile << "signal reqR, ackR, reqL, ackL : std_logic_vector( " << num_reqs-1 << " downto 0);" << endl;
      ofile << "signal reqR_unguarded, ackR_unguarded, reqL_unguarded, ackL_unguarded : std_logic_vector( " << num_reqs-1 << " downto 0);" << endl;
      ofile << "signal reqL_unregulated, ackL_unregulated: std_logic_vector( " << num_reqs-1 << " downto 0);" << endl;
      ofile << "signal guard_vector : std_logic_vector( " << num_reqs-1 << " downto 0);" << endl;
      //ofile << buffering_string  <<  endl;
      ofile << input_buffering_string  <<  endl;
      ofile << output_buffering_string  <<  endl;
      ofile << guard_flags << endl;
      ofile << guard_buffering << endl;
      ofile << "-- }\n begin -- {" << endl;

      // logging!
      if(vcSystem::_enable_logging)
	{
	  ofile << "-- logging on!" << endl;
	  for(int u = 0; u < inwires.size(); u++)
	    {

	      string lrr = reqL[u];
	      string lra = ackL[u];
	      string lcr = reqR[u];
	      string lca = ackR[u];
	      vcWire* iw = inwires[u];
	      vcWire* ow = outwires[u];

	      ofile << "LogMemReadLevel(clk, reset, global_clock_cycle_count,-- { "  << endl
		    << lrr << "," << endl
		    << lra << "," << endl
		    << lcr << "," << endl
		    << lca << "," << endl
		    << "\"" << elements[u] << "\"," << endl
		    << "\"" << ms->Get_VHDL_Id() << "\" ," << endl
		    << ow->Get_VHDL_Signal_Id() << "," << endl
		    << iw->Get_VHDL_Signal_Id() << "," << endl
		    << "\"" << ow->Get_VHDL_Signal_Id() << "\"," << endl
		    << "\"" << iw->Get_VHDL_Signal_Id() << "\" -- } " << endl
		    << ");" << endl;
	    }
	}


      Print_VHDL_Concatenate_Req("reqL_unguarded",reqL,ofile);
      Print_VHDL_Disconcatenate_Ack("ackL_unguarded",ackL,ofile);
      Print_VHDL_Concatenate_Req("reqR_unguarded",reqR,ofile);
      Print_VHDL_Disconcatenate_Ack("ackR_unguarded",ackR,ofile);

      // prepare guard vector.
      Print_VHDL_Guard_Concatenation(num_reqs, "guard_vector", guard_wires, guard_complements, ofile);

      // access regulator: limits the number of pending requests to the operator
      // from a particular request point.
      this->Print_VHDL_Regulator_Instance_Level(group_name + "_accessRegulator", num_reqs, "reqL_unregulated", "ackL_unregulated", "reqL", "ackL", "reqR", "ackR", dpe_elements, ofile);

      Print_VHDL_Guard_Instance_Level(false,false,group_name + "_gI",num_reqs,"guardBuffering","guardFlags","guard_vector",
		      "reqL_unguarded", "ackL_unguarded",
		      "reqL_unregulated", "ackL_unregulated",
		      "reqR_unguarded", "ackR_unguarded",
		      "reqR", "ackR", ofile);

      // concatenate data_in
      Print_VHDL_Concatenation(string("data_in"), inwires,ofile);

      // disconcatenate data_out
      Print_VHDL_Disconcatenation(string("data_out"), out_width, outwires,ofile);

      vcModule* m = this->Get_Parent();

      // now the operator instances 
      string name = '"' + group_name + '"';

      ofile << "LoadReq: LoadReqSharedWithInputBuffersLevel -- {" << endl;
      ofile << "generic map ( name => " << name << ", addr_width => " << addr_width << "," << endl
	      << "  num_reqs => " << num_reqs << "," << endl
	      << "  tag_length => " << tag_length << "," << endl
	      << "  time_stamp_width => " << ms->Calculate_Time_Stamp_Width() << "," << endl
	      << " min_clock_period => " << (vcSystem::_min_clock_period_flag ? "true" : "false") << "," << endl
	      << " input_buffering => inBUFs, " << endl
	      << "  no_arbitration => " << no_arb_string << ")" << endl;
      ofile << "port map ( -- { \n reqL => reqL " << ", " <<  endl
	      << "    ackL => ackL " << ", " <<  endl
	      << "    dataL => data_in, " << endl
	      << "    mreq => " 
	      << ms->Get_VHDL_Memory_Interface_Port_Section(m,"load", "lr_req", idx)  << "," << endl
	      << "    mack => " 
	      << ms->Get_VHDL_Memory_Interface_Port_Section(m, "load", "lr_ack", idx)  << "," << endl
	      << "    maddr => " 
	      << ms->Get_VHDL_Memory_Interface_Port_Section(m, "load", "lr_addr",idx) << "," << endl
	      << "    mtag => "  
	      << ms->Get_VHDL_Memory_Interface_Port_Section(m , "load", "lr_tag",idx) << "," << endl
	      << "  clk => clk, reset => reset -- }\n); -- }" << endl;

      ofile << "LoadComplete: LoadCompleteSharedLevel -- {" << endl;
      ofile << "generic map ( name => " << '"' <<  group_name 
	      << " load-complete "  << '"' << "," << endl
	      << " data_width => " << data_width << "," << endl
	      << "  num_reqs => " << num_reqs << "," << endl
	      << "  tag_length => " << tag_length << "," << endl
	      << "  detailed_buffering_per_output => outBUFs, " << endl
	      << "  no_arbitration => " << no_arb_string << ")" << endl;
      ofile << "port map ( -- {\n reqR => reqR " << ", " <<  endl
	      << "    ackR => ackR " << ", " <<  endl
	      << "    dataR => data_out, " << endl
	      << "    mreq => " 
	      << ms->Get_VHDL_Memory_Interface_Port_Section(m,"load", "lc_req", idx)  
	      << "," << endl
	      << "    mack => " 
	      << ms->Get_VHDL_Memory_Interface_Port_Section(m,"load", "lc_ack", idx)  
	      << "," << endl
	      << "    mdata => " 
	      << ms->Get_VHDL_Memory_Interface_Port_Section(m,"load", "lc_data", idx)  << "," << endl
	      << "    mtag => "  
	      << ms->Get_VHDL_Memory_Interface_Port_Section(m, "load", "lc_tag", idx)  << "," << endl
	      << "  clk => clk, reset => reset -- }\n); -- }" << endl;
      ofile << "-- }\n end Block; -- load group " << idx << endl; // thats it..
    }
}


void vcDataPath::Print_VHDL_Store_Instances_Level(ostream& ofile)
{ 
  string parent_name = this->Get_Parent()->Get_Id();
	string no_arb_string = (vcSystem::_min_area_flag ? "false" : "true");
  if(this->Get_Parent()->Get_Volatile_Flag() && (this->_compatible_store_groups.size() > 0))
  {
	vcSystem::Error("volatile module " + this->Get_Parent()->Get_VHDL_Id() + " cannot have stores.");
	return;
  }

	for(int idx = 0; idx < this->_compatible_store_groups.size(); idx++)
	{ // for each operator group.

		// number of requesters.
		int num_reqs = _compatible_store_groups[idx].size();

		vector<vcWire*> addrwires;
		vector<int> inwire_buffering;

		vector<vcWire*> datawires;
		vector<int> outwire_buffering;

		vector<string> reqL;
		vector<string> ackL;
		vector<string> reqR;
		vector<string> ackR;

		vector<vcWire*> guard_wires;
		vector<bool> guard_complements;

		// to keep track of the ids of the operators in this group.
		vector<string> elements;
		vector<vcDatapathElement*> dpe_elements;

		vcMemorySpace* ms = NULL;

		// ok. collect all the information..
		for(set<vcDatapathElement*>::iterator iter = _compatible_store_groups[idx].begin();
				iter != _compatible_store_groups[idx].end();
				iter++)
		{
			assert((*iter)->Is("vcStore"));
			vcStore* so = (vcStore*) (*iter);
			if(vcSystem::_enable_logging)
				so->vcSplitOperator::Print_VHDL_Logger(this->Get_Parent(), ofile);

			if(ms == NULL)
				ms = so->Get_Memory_Space();
			else
				assert(ms == so->Get_Memory_Space());

			elements.push_back(so->Get_VHDL_Id());
			dpe_elements.push_back(so);

			addrwires.push_back(so->Get_Address());
			datawires.push_back(so->Get_Data());


			// input buffering will be max of addr/data.
			int addr_ibuf = so->Get_Input_Buffering(so->Get_Address());
			int data_ibuf = so->Get_Input_Buffering(so->Get_Data());
			int ibuf  = ((addr_ibuf < data_ibuf) ? data_ibuf : addr_ibuf);
			inwire_buffering.push_back(ibuf);


			//
			// default out buffering on Store complete will be 1, over-ridden by
			// pipeline depth.
			//
			int default_op_buffering = 1;
			{
				int pl_depth, pl_buffering;
				if(so->Is_Part_Of_Pipelined_Loop(pl_depth, pl_buffering))
					default_op_buffering = pl_depth;
			}

			// output buffering  set above..
			outwire_buffering.push_back(default_op_buffering);

			reqL.push_back(so->Get_Req_Level(0));
			ackL.push_back(so->Get_Ack_Level(0));
			reqR.push_back(so->Get_Req_Level(1));
			ackR.push_back(so->Get_Ack_Level(1));

			so->Append_Guard(guard_wires,guard_complements);
		}
		assert(ms != NULL);

		// string buffering_string;
		// this->Generate_Buffering_Constant_Declaration(dpe_elements, buffering_string);


		// logging!
		if(vcSystem::_enable_logging)
		{

			ofile << "-- logging on!" << endl;
			for(int u = 0; u < addrwires.size(); u++)
			{

				string srr = reqL[u];
				string sra = ackL[u];
				string scr = reqR[u];
				string sca = ackR[u];

				vcWire* aw = addrwires[u];
				vcWire* dw = datawires[u];


				ofile << "LogMemWriteLevel(clk, reset,global_clock_cycle_count,  -- { "  << endl
					<< srr << "," << endl
					<< sra << "," << endl
					<< scr << "," << endl
					<< sca << "," << endl
					<< "\"" << elements[u] << "\"," << endl
					<< "\"" << ms->Get_VHDL_Id() << "\" ," << endl
					<< dw->Get_VHDL_Signal_Id() << "," << endl
					<< aw->Get_VHDL_Signal_Id() << "," << endl
					<< "\"" << dw->Get_VHDL_Signal_Id() << "\"," << endl
					<< "\"" << aw->Get_VHDL_Signal_Id() << "\" -- } " << endl
					<< ");" << endl;
			}
		}

		// address, data
		int addr_width = ((vcLoadStore*)(*_compatible_store_groups[idx].begin()))->Get_Address()->Get_Size();
		assert(addr_width == ms->Get_Address_Width());
		int data_width = ((vcLoadStore*)(*_compatible_store_groups[idx].begin()))->Get_Data()->Get_Size();
		assert(data_width == ms->Get_Word_Size());

		// tag-length
		int tag_length = ms->Get_Tag_Length();

		// total addr-width 
		int total_addr_width = addrwires.size() * addr_width;
		int total_data_width = datawires.size() * data_width;

		// VHDL code for this shared group
		ofile << "-- shared store operator group (" << idx << ") : " ;
		for(int u = 0; u < elements.size(); u++)
			ofile << elements[u] << " ";
		ofile << endl;


		string input_buffering_string;
		int maxipb = this->Generate_Buffering_String("inBUFs", inwire_buffering, input_buffering_string);

		string output_buffering_string;
		int maxopb = this->Generate_Buffering_String("outBUFs", outwire_buffering, output_buffering_string);

		string guard_flags;
		string guard_buffering;
		Generate_Guard_Constants(guard_buffering, guard_flags, dpe_elements, guard_wires);

		string group_name = "StoreGroup" + IntToStr(idx);

		// make a block
		ofile << group_name << ": Block -- {" << endl;
		// in and out data.
		ofile << "signal addr_in: std_logic_vector(" << total_addr_width-1 << " downto 0);" << endl;
		ofile << "signal data_in: std_logic_vector(" << total_data_width-1 << " downto 0);" << endl;
		// in and out acks.
		ofile << "signal reqR, ackR, reqL, ackL : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal reqR_unguarded, ackR_unguarded, reqL_unguarded, ackL_unguarded : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal reqL_unregulated, ackL_unregulated : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal guard_vector : std_logic_vector( " << num_reqs-1 << " downto 0);" << endl;
		//ofile << buffering_string << endl;
		ofile << input_buffering_string  <<  endl;
		ofile << output_buffering_string  <<  endl;
		ofile << guard_flags << endl;
		ofile << guard_buffering << endl;

		ofile << "-- }\n begin -- {" << endl;

		Print_VHDL_Concatenate_Req("reqL_unguarded",reqL,ofile);
		Print_VHDL_Disconcatenate_Ack("ackL_unguarded",ackL,ofile);
		Print_VHDL_Concatenate_Req("reqR_unguarded",reqR,ofile);
		Print_VHDL_Disconcatenate_Ack("ackR_unguarded",ackR,ofile);

		// prepare guard vector..
		Print_VHDL_Guard_Concatenation(num_reqs, "guard_vector", guard_wires, guard_complements, ofile);


		this->Print_VHDL_Regulator_Instance_Level(group_name + "_accessRegulator", num_reqs, "reqL_unregulated", "ackL_unregulated", "reqL", "ackL", "reqR", "ackR", dpe_elements, ofile);


		Print_VHDL_Guard_Instance_Level(false,false,group_name + "_gI",num_reqs,"guardBuffering","guardFlags","guard_vector",
				"reqL_unguarded", "ackL_unguarded",
				"reqL_unregulated", "ackL_unregulated",
				"reqR_unguarded", "ackR_unguarded",
				"reqR", "ackR", ofile);

		// concatenate addr_in and data_in
		Print_VHDL_Concatenation(string("addr_in"), addrwires,ofile);
		Print_VHDL_Concatenation(string("data_in"), datawires,ofile);

		vcModule* m = this->Get_Parent();

		string sr_name = '"' + group_name + " Req " + '"';
		// now the operator instances 
		ofile << "StoreReq: StoreReqSharedWithInputBuffers -- {" << endl;
		ofile << "generic map ( name => " << sr_name << ", addr_width => " << addr_width << "," << endl
			<< "  data_width => " << data_width << "," << endl
			<< "  num_reqs => " << num_reqs << "," << endl
			<< "  tag_length => " << tag_length << "," << endl
			<< "  time_stamp_width => " << ms->Calculate_Time_Stamp_Width() << "," << endl
			<< "  min_clock_period => " << (vcSystem::_min_clock_period_flag ? "true" : "false") << "," << endl
			<< "  input_buffering => inBUFs, " << endl
			<< "  no_arbitration => " << no_arb_string << ")" << endl;
		ofile << "port map (--{\n reqL => reqL " << ", " <<  endl
			<< "    ackL => ackL " << ", " <<  endl
			<< "    addr => addr_in, " << endl
			<< "    data => data_in, " << endl
			<< "    mreq => " 
			<< ms->Get_VHDL_Memory_Interface_Port_Section(m,"store", "sr_req", idx)  << "," << endl
			<< "    mack => " 
			<< ms->Get_VHDL_Memory_Interface_Port_Section(m,"store", "sr_ack", idx)  << "," << endl
			<< "    maddr => " 
			<< ms->Get_VHDL_Memory_Interface_Port_Section(m,"store","sr_addr",idx) << "," << endl
			<< "    mdata => " 
			<< ms->Get_VHDL_Memory_Interface_Port_Section(m,"store","sr_data",idx) << "," << endl
			<< "    mtag => "  
			<< ms->Get_VHDL_Memory_Interface_Port_Section(m,"store","sr_tag",idx) << "," << endl
			<< "  clk => clk, reset => reset -- }\n);--}" << endl;

		string sc_name = '"' + group_name + " Complete " + '"';
		ofile << "StoreComplete: StoreCompleteShared -- { " << endl;
		ofile << "generic map ( -- { "
			<< " name => " << sc_name  << "," << endl
			<< "  num_reqs => " << num_reqs << "," << endl
			<< "  detailed_buffering_per_output => outBUFs," << endl
			<< "  tag_length => " << tag_length << " -- } \n)" << endl;
		ofile << "port map ( -- { reqR => reqR " << ", " <<  endl
			<< "    ackR => ackR " << ", " <<  endl
			<< "    mreq => " 
			<< ms->Get_VHDL_Memory_Interface_Port_Section(m,"store", "sc_req", idx)  << "," << endl
			<< "    mack => " 
			<< ms->Get_VHDL_Memory_Interface_Port_Section(m,"store", "sc_ack", idx)  << "," << endl
			<< "    mtag => "  
			<< ms->Get_VHDL_Memory_Interface_Port_Section(m,"store", "sc_tag", idx)  << "," << endl
			<< "  clk => clk, reset => reset -- }); -- }" << endl;

		ofile << "-- }\n end Block; -- store group " << idx << endl; // thats it..
	}
}

void vcDataPath::Print_VHDL_Inport_Instances(ostream& ofile)
{

	string no_arb_string = (vcSystem::_min_area_flag ? "false" : "true");
	string parent_name = this->Get_Parent()->Get_Id();

	if(this->Get_Parent()->Get_Volatile_Flag() && (this->_compatible_inport_groups.size() > 0))
	{
		vcSystem::Error("volatile module " + this->Get_Parent()->Get_VHDL_Id() + " cannot interact with pipes.");
		return;
	}

	for(int idx = 0; idx < this->_compatible_inport_groups.size(); idx++)
	{ // for each operator group.

		// number of requesters.
		int num_reqs = _compatible_inport_groups[idx].size();

		vector<string> reqL;
		vector<string> ackL;
		vector<string> reqR;
		vector<string> ackR;

		vector<vcWire*> outwires;
		vector<int> outwire_buffering;

		vector<vcWire*> guards;
		vector<vcDatapathElement*> dpe_elements;
		vector<bool> guard_complements;

		// to keep track of the ids of the operators in this group.
		vector<string> elements;


		vcPipe* p = NULL;
		int data_width = -1;

		// ok. collect all the information..
		for(set<vcDatapathElement*>::iterator iter = _compatible_inport_groups[idx].begin();
				iter != _compatible_inport_groups[idx].end();
				iter++)
		{

			assert((*iter)->Is("vcInport"));
			vcInport* so = (vcInport*) (*iter);
			if(vcSystem::_enable_logging)
				so->Print_VHDL_Logger(this->Get_Parent(), ofile);

			if(p == NULL)
				p = ((vcInport*) so)->Get_Pipe();
			else
				assert(p == ((vcInport*) so)->Get_Pipe());

			if(data_width < 0)
				data_width = so->Get_Data()->Get_Size();
			else
				assert(data_width == so->Get_Data()->Get_Size());

			elements.push_back(so->Get_VHDL_Id());
			dpe_elements.push_back(so);
			so->Append_Outwires(outwires);
			so->Append_Outwire_Buffering(outwire_buffering, num_reqs);

			reqL.push_back(so->Get_Req_Level(0));
			ackL.push_back(so->Get_Ack_Level(0));
			reqR.push_back(so->Get_Req_Level(1));
			ackR.push_back(so->Get_Ack_Level(1));

			so->Append_Guard(guards, guard_complements);
		}
		assert(p != NULL);
		assert(data_width > 0);



		// total out-width..
		int out_width = 0;
		for(int u = 0; u < outwires.size(); u++)
		{
			out_width += outwires[u]->Get_Size();
		}

		// if p is a signal, read it off immediately.
		if(p->Get_Signal())
		{
			ofile << "-- read from input-signal " << p->Get_Id() << endl;
			for(int u = 0; u < outwires.size(); u++)
			{
				ofile << outwires[u]->Get_VHDL_Id() << " <= " << p->Get_Id() << ";" << endl;
			}
			continue;
		} 

		// VHDL code for this shared group
		ofile << "-- shared inport operator group (" << idx << ") : " ;
		for(int u = 0; u < elements.size(); u++)
			ofile << elements[u] << " ";
		ofile << endl;

		string output_buffering_string;
		int maxopb = this->Generate_Buffering_String("outBUFs", outwire_buffering, output_buffering_string);

		string guard_flags;
		string guard_buffering;
		Generate_Guard_Constants(guard_buffering, guard_flags, dpe_elements, guards);

		// make a block
		string block_name = "InportGroup_" + IntToStr(idx);
		ofile << block_name << ": Block -- {" << endl;
		// in and out data.
		ofile << "signal data_out: std_logic_vector(" << out_width-1 << " downto 0);" << endl;
		// in and out acks.
		ofile << "signal reqL, ackL, reqR, ackR : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal reqL_unguarded, ackL_unguarded : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal reqR_unguarded, ackR_unguarded : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal guard_vector : std_logic_vector( " << num_reqs-1 << " downto 0);" << endl;

		ofile << output_buffering_string << endl;
		ofile << guard_flags << endl;
		ofile << guard_buffering << endl;

		ofile << "-- }\n begin -- {" << endl;


		// guard related stuff.
		Print_VHDL_Concatenate_Req("reqL_unguarded",reqL,ofile);
		Print_VHDL_Disconcatenate_Ack("ackL_unguarded",ackL,ofile);
		Print_VHDL_Concatenate_Req("reqR_unguarded",reqR,ofile);
		Print_VHDL_Disconcatenate_Ack("ackR_unguarded",ackR,ofile);
		Print_VHDL_Guard_Concatenation(num_reqs, "guard_vector", guards, guard_complements, ofile);


		// disconcatenate data_out
		Print_VHDL_Disconcatenation(string("data_out"), out_width, outwires,ofile);

		// now the operator instances 
		string group_name = p->Get_VHDL_Id() + "_read_" + IntToStr(idx);
		string name = '"' + group_name + '"';

		Print_VHDL_Guard_Instance_Level(false, true, group_name + "_gI",num_reqs,"guardBuffering","guardFlags","guard_vector",
				"reqL_unguarded", "ackL_unguarded",
				"reqL", "ackL",
				"reqR_unguarded", "ackR_unguarded",
				"reqR", "ackR", ofile);

		bool p2p_flag = p->Get_P2P();
		bool noblock_flag = p->Get_No_Block_Mode();
		bool bypass = p->Get_Bypass();
		if(p2p_flag)
		{
			if(num_reqs > 1)
			{
				vcSystem::Error("P2P pipe " + p->Get_VHDL_Id() + " cannot have multiple readers. ");
				assert(0);
			}

			int fifo_register_bits = data_width * p->Get_Depth();
			vcSystem::Increment_Fifo_Register_Count (fifo_register_bits);

			ofile << group_name << ": InputPort_P2P -- { " << endl;
			ofile << "generic map ( name => " << name << ", data_width => " << data_width << ","
				<< "    bypass_flag => " << (bypass ? "true" : "false") << ","
				<< "   	nonblocking_read_flag => " << (noblock_flag ? "true" : "false") << "," 
				<< "  queue_depth =>  " << p->Get_Depth() << ")" << endl;
			ofile << "port map (-- {\n sample_req => reqL(0) " << ", " <<  endl
				<< "    sample_ack => ackL(0)" << ", " <<  endl
				<< "    update_req => reqR(0)" << ", " <<  endl
				<< "    update_ack => ackR(0)" << ", " <<  endl
				<< "    data => data_out, " << endl
				<< "    oreq => " 
				<< this->Get_VHDL_IOport_Interface_Port_Section(p, "in" , "read_req", idx)  << "," << endl
				<< "    oack => " 
				<< this->Get_VHDL_IOport_Interface_Port_Section(p,"in",  "read_ack", idx)  << "," << endl
				<< "    odata => " 
				<< this->Get_VHDL_IOport_Interface_Port_Section(p,"in", "read_data", idx)  << "," << endl
				<< "  clk => clk, reset => reset -- }\n ); -- }" << endl;
			ofile << "-- }\n end Block; -- inport group " << idx << endl; // thats it..

		}
		else
		{
			ofile << group_name << ": InputPortRevised -- { " << endl;
			ofile << "generic map ( name => " << name << ", data_width => " << data_width << ","
				<< "  num_reqs => " << num_reqs << ","
				<< "  output_buffering => outBUFs, "
				<< "  nonblocking_read_flag => " << (noblock_flag ? "true" : "False") << "," 
				<< "  no_arbitration => " << no_arb_string << ")" << endl;
			ofile << "port map (-- {\n sample_req => reqL " << ", " <<  endl
				<< "    sample_ack => ackL" << ", " <<  endl
				<< "    update_req => reqR" << ", " <<  endl
				<< "    update_ack => ackR" << ", " <<  endl
				<< "    data => data_out, " << endl
				<< "    oreq => " 
				<< this->Get_VHDL_IOport_Interface_Port_Section(p, "in" , "read_req", idx)  << "," << endl
				<< "    oack => " 
				<< this->Get_VHDL_IOport_Interface_Port_Section(p,"in",  "read_ack", idx)  << "," << endl
				<< "    odata => " 
				<< this->Get_VHDL_IOport_Interface_Port_Section(p,"in", "read_data", idx)  << "," << endl
				<< "  clk => clk, reset => reset -- }\n ); -- }" << endl;
			ofile << "-- }\n end Block; -- inport group " << idx << endl; // thats it..
		}
	}
}


void vcDataPath::Print_VHDL_Outport_Instances(ostream& ofile)
{ 
	string no_arb_string = (vcSystem::_min_area_flag ? "false" : "true");
	string parent_name = this->Get_Parent()->Get_Id();
	if(this->Get_Parent()->Get_Volatile_Flag() && (this->_compatible_outport_groups.size() > 0))
	{
		vcSystem::Error("volatile module " + this->Get_Parent()->Get_VHDL_Id() + " cannot interact with pipes.");
		return;
	}

	for(int idx = 0; idx < this->_compatible_outport_groups.size(); idx++)
	{ // for each operator group.

		// number of requesters.
		int num_reqs = _compatible_outport_groups[idx].size();

		vector<vcTransition*> sample_req;
		vector<vcTransition*> sample_ack;
		vector<vcTransition*> update_req;
		vector<vcTransition*> update_ack;

		vector<vcWire*> inwires;
		vector<int> inwire_buffering;

		vector<vcWire*> guards;
		vector<bool> guard_complements;

		vector<vcDatapathElement*> dpe_elements;

		// to keep track of the ids of the operators in this group.
		vector<string> elements;

		vcPipe* p = NULL;
		int data_width = -1;

		vcDatapathElement* lo = *(_compatible_outport_groups[idx].begin());
		bool full_rate = lo->Get_Full_Rate();

		// ok. collect all the information..
		for(set<vcDatapathElement*>::iterator iter = _compatible_outport_groups[idx].begin();
				iter != _compatible_outport_groups[idx].end();
				iter++)
		{

			assert((*iter)->Is("vcOutport"));
			vcOutport* so = (vcOutport*) (*iter);
			if(vcSystem::_enable_logging)
				so->Print_VHDL_Logger(this->Get_Parent(), ofile);

			if(p == NULL)
				p = ((vcOutport*) so)->Get_Pipe();
			else
				assert(p == ((vcOutport*) so)->Get_Pipe());

			if(data_width < 0)
				data_width = so->Get_Data()->Get_Size();
			else
				assert(data_width == so->Get_Data()->Get_Size());

			elements.push_back(so->Get_VHDL_Id());
			dpe_elements.push_back(so);

			so->Append_Inwires(inwires);
			so->Append_Inwire_Buffering(inwire_buffering);

			sample_req.push_back(so->Get_Req(0));
			sample_ack.push_back(so->Get_Ack(0));

			update_req.push_back(so->Get_Req(1));
			update_ack.push_back(so->Get_Ack(1));

			so->Append_Guard(guards, guard_complements);
		}
		assert(p != NULL);
		assert(data_width > 0);

		int in_width = 0;
		for(int u = 0; u < inwires.size(); u++)
		{
			in_width += inwires[u]->Get_Size();
		}


		string input_buffering_string;
		int max_inbuf = this->Generate_Buffering_String("inBUFs", inwire_buffering, input_buffering_string);

		string guard_flags;
		string guard_buffering;
		Generate_Guard_Constants(guard_buffering, guard_flags, dpe_elements, guards);

		// VHDL code for this shared group
		ofile << "-- shared outport operator group (" << idx << ") : " ;
		for(int u = 0; u < elements.size(); u++)
			ofile << elements[u] << " ";
		ofile << endl;


		// make a block
		string block_name  = "OutportGroup_" + IntToStr(idx);
		ofile << block_name << ": Block -- { " << endl;
		// in and out data.
		ofile << "signal data_in: std_logic_vector(" << in_width-1 << " downto 0);" << endl;
		// in and out acks.
		ofile << "signal sample_req, sample_ack : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal update_req, update_ack : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal sample_req_unguarded, sample_ack_unguarded : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal update_req_unguarded, update_ack_unguarded : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;

		ofile << "signal guard_vector : std_logic_vector( " << num_reqs-1 << " downto 0);" << endl;

		ofile << input_buffering_string << endl;
		ofile << guard_flags << endl;
		ofile << guard_buffering << endl;
		ofile << "-- }\n begin -- {" << endl;

		// guard related stuff.
		Print_VHDL_Concatenate_Req("sample_req_unguarded",sample_req,ofile);
		Print_VHDL_Disconcatenate_Ack("sample_ack_unguarded",sample_ack,ofile);
		Print_VHDL_Concatenate_Req("update_req_unguarded",update_req,ofile);
		Print_VHDL_Disconcatenate_Ack("update_ack_unguarded",update_ack,ofile);
		Print_VHDL_Guard_Concatenation(num_reqs, "guard_vector", guards, guard_complements, ofile);

		// concatenate data_in
		Print_VHDL_Concatenation(string("data_in"), inwires,ofile);

		// now the operator instances 
		string group_name = p->Get_VHDL_Id() + "_write_" + IntToStr(idx);
		string name = '"' + p->Get_VHDL_Id() + '"';

		Print_VHDL_Guard_Instance_Level(true, false, group_name + "_gI",num_reqs,"guardBuffering","guardFlags","guard_vector",
				"sample_req_unguarded", "sample_ack_unguarded",
				"sample_req", "sample_ack",
				"update_req_unguarded", "update_ack_unguarded",
				"update_req", "update_ack", ofile);
		bool p2p_flag = p->Get_P2P();
		if(p2p_flag)
		{
			if(num_reqs > 1)
			{
				vcSystem::Warning("P2P pipe " + p->Get_VHDL_Id() + " has multiple writers. ");
			}
		}

		ofile << group_name << ": OutputPortRevised -- { " << endl;
		ofile << "generic map ( name => " << name << ", data_width => " << data_width << ","
			<< " num_reqs => " << num_reqs << ","
			<< " input_buffering => inBUFs," 
			<< " full_rate => " << (full_rate ? "true," : "false,") << endl
			<< " no_arbitration => " << no_arb_string << ")" << endl;
		ofile << "port map (--{\n sample_req => sample_req " << ", " <<  endl
			<< "    sample_ack => sample_ack " << ", " <<  endl
			<< "    update_req => update_req " << ", " <<  endl
			<< "    update_ack => update_ack " << ", " <<  endl
			<< "    data => data_in, " << endl
			<< "    oreq => " 
			<< this->Get_VHDL_IOport_Interface_Port_Section(p,"out" , "write_req", idx)  << "," << endl
			<< "    oack => " 
			<< this->Get_VHDL_IOport_Interface_Port_Section(p,"out",  "write_ack", idx)  << "," << endl
			<< "    odata => " 
			<< this->Get_VHDL_IOport_Interface_Port_Section(p,"out", "write_data", idx)  << "," << endl
			<< "  clk => clk, reset => reset -- }\n);-- }" << endl;
		ofile << "-- }\n end Block; -- outport group " << idx << endl; // thats it..
	}
}


string vcDataPath::Get_VHDL_IOport_Interface_Port_Name(string pipe_id, string pid)
{
	return(this->Get_Parent()->Get_Parent()->Get_VHDL_Pipe_Interface_Port_Name(pipe_id,pid));
}

string vcDataPath::Get_VHDL_IOport_Interface_Port_Section(vcPipe* p, 
		string in_or_out,
		string pid,
		int idx)
{

	string pipe_id = p->Get_Id();
	string port_name = p->Get_VHDL_Pipe_Interface_Port_Name(pid);

	vcModule* m = this->Get_Parent();
	vcSystem* sys = m->Get_Parent();

	map<vcPipe*,vector<int> >::iterator iter;
	if(in_or_out == "in")
	{
		iter = _inport_group_map.find(p);
		assert(iter != _inport_group_map.end());
	}
	else
	{
		iter = _outport_group_map.find(p);
		assert(iter != _outport_group_map.end());
	}

	int down_index;
	// left to right 
	for(int index = 0; index < (*iter).second.size(); index++)
	{
		down_index = ((*iter).second.size()-1) - index; // position from left.
		if(idx == (*iter).second[index])
			break;
		if(index == (*iter).second.size() - 1)
			assert(0);
	}


	int pipe_width = p->Get_Width();

	if((pid.find("req") != string::npos) || (pid.find("ack") != string::npos))
		return(port_name +  "(" + IntToStr(down_index) + ")");
	else if(pid.find("data") != string::npos)
		return(port_name + "(" 
				+ IntToStr(((down_index+1)*pipe_width)-1) + " downto "
				+ IntToStr(down_index*pipe_width) + ")");
	else
		assert(0); // fatal
}


void vcDataPath::Print_VHDL_Call_Instances(ostream& ofile)
{ 

	string no_arb_string = (vcSystem::_min_area_flag ? "false" : "true");
	string parent_name = this->Get_Parent()->Get_Id();

	for(int idx = 0; idx < this->_compatible_call_groups.size(); idx++)
	{ // for each operator group.

		// number of requesters.
		int num_reqs = _compatible_call_groups[idx].size();

		// to collect inwires, outwires and reqs/acks.
		vector<vcWire*> inwires;
		vector<int> inwire_buffering;

		vector<vcTransition*> reqL;
		vector<vcTransition*> ackL;
		vector<vcTransition*> reqR;
		vector<vcTransition*> ackR;

		vector<vcWire*> outwires;
		vector<int> outwire_buffering;

		vector<vcWire*> guard_wires;
		vector<bool> guard_complements;

		// to keep track of the ids of the operators in this group.
		vector<string> elements;
		vector<vcDatapathElement*> dpe_elements;

		vcModule* called_module = NULL;

		bool skip_because_volatile = false;
		bool skip_because_operator = false;


		// lead operator.. is it full-rate?
		vcDatapathElement* lo = *(_compatible_call_groups[idx].begin());
		bool full_rate = lo->Get_Full_Rate();

		// ok. collect all the information..
		for(set<vcDatapathElement*>::iterator iter = _compatible_call_groups[idx].begin();
				iter != _compatible_call_groups[idx].end();
				iter++)
		{

			assert((*iter)->Is("vcCall"));
			vcCall* so = (vcCall*) (*iter);
			if(vcSystem::_enable_logging)
				so->vcSplitOperator::Print_VHDL_Logger(this->Get_Parent(), ofile);

			if(called_module == NULL)
				called_module = so->Get_Called_Module();
			else
				assert(called_module == so->Get_Called_Module());


			if((this->Get_Parent()->Get_Volatile_Flag() || so->Get_Flow_Through()) &&
					!called_module->Get_Volatile_Flag())
			{
				vcSystem::Error("call to a non-volatile module " + called_module->Get_VHDL_Id() +
						" from a volatile module/call-point in " + this->Get_Parent()->Get_VHDL_Id());
				continue;
			}

			if(this->Get_Parent()->Get_Operator_Flag() &&
					!(called_module->Get_Volatile_Flag() || called_module->Get_Operator_Flag()))
			{
				vcSystem::Error("call to a non-volatile/non-operator module " + called_module->Get_VHDL_Id() +
						" from an operator module " + this->Get_Parent()->Get_VHDL_Id());
				continue;
			}


			if(so->Get_Flow_Through() || called_module->Get_Volatile_Flag())
			{
				so->Print_Flow_Through_VHDL(ofile);
				skip_because_volatile = true;
			}
			else if(called_module->Get_Operator_Flag())
			{
				so->Print_Operator_VHDL(ofile);
				skip_because_operator = true;
			}

			if(skip_because_volatile || skip_because_operator)
			{
				// group cannot have more than one member.
				if(_compatible_call_groups[idx].size() > 1)
				{
					vcSystem::Error(
							"FATAL: call-group to  non-volatile/non-operator module  has more than one member in caller module "  
							+ this->Get_Parent()->Get_VHDL_Id());
				}
				break;
			}


			elements.push_back(so->Get_VHDL_Id());
			dpe_elements.push_back(so);

			so->Append_Inwires(inwires);
			so->Append_Inwire_Buffering(inwire_buffering);

			so->Append_Outwires(outwires);
			so->Append_Outwire_Buffering(outwire_buffering, num_reqs);

			reqL.push_back(so->Get_Req(0));
			ackL.push_back(so->Get_Ack(0));
			reqR.push_back(so->Get_Req(1));
			ackR.push_back(so->Get_Ack(1));

			so->Append_Guard(guard_wires,guard_complements);
		}
		assert(called_module != NULL);


		if(skip_because_volatile || skip_because_operator)
			continue;

		string input_buffering_string;
		int max_inbuf = this->Generate_Buffering_String("inBUFs", inwire_buffering, input_buffering_string);

		bool use_out_buffering = ((called_module->Get_Out_Arg_Width() > 0) ? true : false);
		string output_buffering_string;
		int max_outbuf;

		// special case.. hack alert.
		if(use_out_buffering)
			max_outbuf  = this->Generate_Buffering_String("outBUFs", outwire_buffering, output_buffering_string);
		else
			output_buffering_string =  "constant outBUFs: IntegerArray(" + IntToStr(num_reqs-1) + " downto 0) := (others => 1);";


		string guard_flags;
		string guard_buffering;
		Generate_Guard_Constants(guard_buffering, guard_flags, dpe_elements, guard_wires);

		int tag_length = called_module->Get_Caller_Tag_Length();

		// total in-width 
		int in_width = 0;
		for(int u = 0; u < inwires.size(); u++)
		{
			in_width += inwires[u]->Get_Size();
		}
		assert(in_width == called_module->Get_In_Arg_Width() * num_reqs);

		// total out-width..
		int out_width = 0;
		for(int u = 0; u < outwires.size(); u++)
		{
			out_width += outwires[u]->Get_Size();
		}
		assert(out_width == called_module->Get_Out_Arg_Width() * num_reqs);

		// VHDL code for this shared group
		ofile << "-- shared call operator group (" << idx << ") : " ;
		for(int u = 0; u < elements.size(); u++)
			ofile << elements[u] << " ";
		ofile << endl;


		// make a block
		string group_name = called_module->Get_VHDL_Id() + "_call_group_" + IntToStr(idx);
		ofile << group_name << ": Block -- {" << endl;
		// in and out data.
		if(called_module->Get_In_Arg_Width() > 0)
			ofile << "signal data_in: std_logic_vector(" << in_width-1 << " downto 0);" << endl;
		if(called_module->Get_Out_Arg_Width() > 0)
			ofile << "signal data_out: std_logic_vector(" << out_width-1 << " downto 0);" << endl;
		// in and out acks.
		ofile << "signal reqR, ackR, reqL, ackL : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal reqR_unguarded, ackR_unguarded, reqL_unguarded, ackL_unguarded : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal reqL_unregulated, ackL_unregulated : BooleanArray( " << num_reqs-1 << " downto 0);" << endl;
		ofile << "signal guard_vector : std_logic_vector( " << num_reqs-1 << " downto 0);" << endl;

		// ofile << buffering_string << endl;
		ofile << input_buffering_string << endl;
		ofile << output_buffering_string << endl;
		ofile << guard_flags << endl;
		ofile << guard_buffering << endl;
		ofile << "-- }\n begin -- {" << endl;

		Print_VHDL_Concatenate_Req("reqL_unguarded",reqL,ofile);
		Print_VHDL_Disconcatenate_Ack("ackL_unguarded",ackL,ofile);
		Print_VHDL_Concatenate_Req("reqR_unguarded",reqR,ofile);
		Print_VHDL_Disconcatenate_Ack("ackR_unguarded",ackR,ofile);

		// prepare guard vector.
		Print_VHDL_Guard_Concatenation(num_reqs, "guard_vector", guard_wires, guard_complements, ofile);


		this->Print_VHDL_Regulator_Instance_Level(group_name + "_accessRegulator", num_reqs,"reqL_unregulated", "ackL_unregulated", "reqL", "ackL", "reqR", "ackR", dpe_elements,  ofile);

		Print_VHDL_Guard_Instance_Level(false, false, group_name + "_gI",num_reqs,"guardBuffering","guardFlags","guard_vector",
				"reqL_unguarded", "ackL_unguarded",
				"reqL_unregulated", "ackL_unregulated",
				"reqR_unguarded", "ackR_unguarded",
				"reqR", "ackR", ofile);

		// concatenate data_in
		if(called_module->Get_In_Arg_Width() > 0)
			Print_VHDL_Concatenation(string("data_in"), inwires,ofile);

		// disconcatenate data_out
		if(called_module->Get_Out_Arg_Width() > 0)
			Print_VHDL_Disconcatenation(string("data_out"), out_width, outwires,ofile);

		vcModule* m = this->Get_Parent();
		// now the operator instances 
		string imux_name = ((called_module->Get_In_Arg_Width() > 0) ?
				"InputMuxWithBuffering" : "InputMuxBaseNoData");
		bool use_in_buffering = (called_module->Get_In_Arg_Width() > 0);

		ofile << "CallReq: " << imux_name << " -- {" << endl;
		ofile << "generic map (name => \"" << imux_name << "\"," << endl;

		if(use_in_buffering)
		{
			ofile << " iwidth => " << in_width << "," << endl
				<< " owidth => " << in_width/num_reqs << "," << endl;

			ofile << " buffering => inBUFs,"  << endl;

			ofile << " full_rate => " << (full_rate ? " true," : " false,") << endl;
		}


		ofile << " twidth => " << tag_length << "," << endl
			<< " nreqs => " << num_reqs << "," << endl;


		if(called_module->Get_In_Arg_Width() > 0)
			ofile << " registered_output => "
				<< (vcSystem::_min_clock_period_flag ? "true" : "false") << "," << endl;


		ofile   << "  no_arbitration => " << no_arb_string << ")" << endl;
		ofile << "port map ( -- { \n reqL => reqL " << ", " <<  endl
			<< "    ackL => ackL " << ", " <<  endl;

		if(called_module->Get_In_Arg_Width() > 0)
			ofile << "    dataL => data_in, " << endl;

		ofile << "    reqR => " 
			<< called_module->Get_VHDL_Call_Interface_Port_Section(m,"call", "call_reqs", idx)  << "," << endl
			<< "    ackR => " 
			<< called_module->Get_VHDL_Call_Interface_Port_Section(m, "call", "call_acks", idx)  << "," << endl;

		if(called_module->Get_In_Arg_Width() > 0)
			ofile << "    dataR => " 
				<< called_module->Get_VHDL_Call_Interface_Port_Section(m, "call", "call_data",idx) << "," << endl;


		ofile << "    tagR => "  
			<< called_module->Get_VHDL_Call_Interface_Port_Section(m , "call", "call_tag",idx) 
			<< "," << endl;

		ofile << "  clk => clk, reset => reset -- }\n); -- }" << endl;


		string omux_name = ((called_module->Get_Out_Arg_Width() > 0) ?
				"OutputDemuxBaseWithBuffering" : "OutputDemuxBaseNoData");

		ofile << "CallComplete: " << omux_name << " -- {" << endl;
		ofile << "generic map ( -- {" << endl;
		if(called_module->Get_Out_Arg_Width() > 0)
		{
			ofile << "iwidth => " << out_width/num_reqs << "," << endl
				<< " owidth => " << out_width << "," << endl; 
		}
		ofile << " detailed_buffering_per_output => outBUFs, " << endl;

		if(called_module->Get_Out_Arg_Width() > 0)
		{
			ofile << " full_rate => " << 
				(full_rate ? "true" : "false") << ", " << endl;
		}

		ofile << " twidth => " << tag_length << "," << endl
			<< " name => " << '"' << omux_name << '"' << "," << endl
			<< " nreqs => " << num_reqs << ") -- }" << endl;
		ofile << "port map ( -- {\n reqR => reqR " << ", " <<  endl
			<< "    ackR => ackR " << ", " <<  endl;

		if(called_module->Get_Out_Arg_Width() > 0)
			ofile << "    dataR => data_out, " << endl;

		ofile << "    reqL => " 
			<< called_module->Get_VHDL_Call_Interface_Port_Section(m,"return", "return_acks", idx)  << ", -- cross-over" << endl
			<< "    ackL => " 
			<< called_module->Get_VHDL_Call_Interface_Port_Section(m,"return", "return_reqs", idx)  << ", -- cross-over" << endl;

		if(called_module->Get_Out_Arg_Width() > 0)
		{
			ofile << "    dataL => " 
				<< called_module->Get_VHDL_Call_Interface_Port_Section(m,"return", "return_data", idx)  << "," << endl;
		}

		ofile << "    tagL => "  
			<< called_module->Get_VHDL_Call_Interface_Port_Section(m, "return", "return_tag", idx)  << "," << endl
			<< "  clk => clk," << endl
			<< " reset => reset -- }\n); -- }" << endl;
		ofile << "-- }\n end Block; -- call group " << idx << endl; // thats it..
	}
}


string vcDataPath::Print_VHDL_Memory_Interface_Port_Map(string comma, ostream& ofile)
{

	// in progress
	set<vcMemorySpace*,vcRoot_Compare> ms_set;
	vcMemorySpace* ms;
	vcModule* parent_module = this->Get_Parent();

	// first the loads
	for(int idx = 0; idx < _compatible_load_groups.size(); idx++)
	{
		ms = ((vcLoad*) (*(_compatible_load_groups[idx].begin())))->Get_Memory_Space();
		ms_set.insert(ms);
	}

	for(set<vcMemorySpace*,vcRoot_Compare>::iterator iter = ms_set.begin();
			iter != ms_set.end();
			iter++)
	{
		ms = (*iter);

		if(ms->Get_Scope() == NULL) // only if ms is at the system level
		{
			// if ms is not inside this module, then print a port map..
			int hindex, lindex;
			if(ms->Get_Caller_Module_Section(parent_module,"load",hindex,lindex))
			{

				if(parent_module->Get_Volatile_Flag())
				{
					vcSystem::Error("volatile module " + parent_module->Get_Label() + " loads from memory space " + ms->Get_VHDL_Id());
					continue;
				}
				else if(parent_module->Get_Operator_Flag())
				{
					vcSystem::Error("operator module " + parent_module->Get_Label() + " loads from external memory space " + ms->Get_VHDL_Id());
					continue;
				}

				ofile << comma << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("lr_req") << " => " <<
					ms->Get_Aggregate_Section("lr_req", hindex, lindex) << "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("lr_ack") << " => " <<
					ms->Get_Aggregate_Section("lr_ack", hindex, lindex) << "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("lr_addr") << " => " << 
					ms->Get_Aggregate_Section("lr_addr", hindex, lindex) << "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("lr_tag") << " => " << 
					ms->Get_Aggregate_Section("lr_tag", hindex, lindex) << "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("lc_req") << " => " << 
					ms->Get_Aggregate_Section("lc_req", hindex, lindex)<< "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("lc_ack") << " => " << 
					ms->Get_Aggregate_Section("lc_ack", hindex, lindex)<< "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("lc_data") << " => " << 
					ms->Get_Aggregate_Section("lc_data", hindex, lindex)<< "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("lc_tag") << " => " << 
					ms->Get_Aggregate_Section("lc_tag", hindex, lindex);
				comma = ",";
			}
		}
	}


	ms_set.clear();

	// now the stores
	for(int idx = 0; idx < _compatible_store_groups.size(); idx++)
	{
		ms = ((vcLoad*) (*(_compatible_store_groups[idx].begin())))->Get_Memory_Space();
		ms_set.insert(ms);
	}

	for(set<vcMemorySpace*,vcRoot_Compare>::iterator iter = ms_set.begin();
			iter != ms_set.end();
			iter++)
	{
		ms = (*iter);

		if(ms->Get_Scope() == NULL) // only if ms is at the system level
		{
			int hindex, lindex;
			if(ms->Get_Caller_Module_Section(parent_module,"store",hindex,lindex))
			{
				if(parent_module->Get_Volatile_Flag())
				{
					vcSystem::Error("volatile module " + parent_module->Get_Label() + " stores to memory space " + ms->Get_VHDL_Id());
					continue;
				}
				else if(parent_module->Get_Operator_Flag())
				{
					vcSystem::Error("operator module " + parent_module->Get_Label() + " stores to external memory space " + ms->Get_VHDL_Id());
					continue;
				}

				ofile << comma << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("sr_req") << " => " <<
					ms->Get_Aggregate_Section("sr_req", hindex, lindex) << "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("sr_ack") << " => " <<
					ms->Get_Aggregate_Section("sr_ack", hindex, lindex) << "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("sr_addr") << " => " << 
					ms->Get_Aggregate_Section("sr_addr", hindex, lindex) << "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("sr_data") << " => " << 
					ms->Get_Aggregate_Section("sr_data", hindex, lindex)<< "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("sr_tag") << " => " << 
					ms->Get_Aggregate_Section("sr_tag", hindex, lindex) << "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("sc_req") << " => " << 
					ms->Get_Aggregate_Section("sc_req", hindex, lindex)<< "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("sc_ack") << " => " << 
					ms->Get_Aggregate_Section("sc_ack", hindex, lindex)<< "," << endl;
				ofile << ms->Get_VHDL_Memory_Interface_Port_Name("sc_tag") << " => " << 
					ms->Get_Aggregate_Section("sc_tag", hindex, lindex);
				comma = ",";
			}
		}
	}
	return(comma);
}

string vcDataPath::Print_VHDL_IO_Interface_Port_Map(string comma, ostream& ofile)
{
	set<vcPipe*> pipe_set;
	string pipe_id;
	vcModule* parent_module = this->Get_Parent();

	for(int idx = 0; idx < _compatible_inport_groups.size(); idx++)
	{
		vcPipe* p = ((vcInport*) (*(_compatible_inport_groups[idx].begin())))->Get_Pipe();
		pipe_set.insert(p);
	}

	for(set<vcPipe*>::iterator iter = pipe_set.begin();
			iter != pipe_set.end();
			iter++)
	{
		vcPipe* p = (*iter);

		if(p->Get_Parent() == NULL)
		{

			if(p->Get_Signal())
			{
				ofile << comma << endl;
				ofile << p->Get_VHDL_Id()  <<  " => "  << p->Get_VHDL_Id();
				comma = "," ;
			}
			else
			{
				int hindex, lindex;
				if(p->Get_Pipe_Module_Section(parent_module,"read", hindex,lindex))
				{
					if(parent_module->Get_Volatile_Flag())
					{
						vcSystem::Error("volatile module " + parent_module->Get_Label() + " reads from pipe " + p->Get_Id());
						continue;
					}
					else if (parent_module->Get_Volatile_Flag())
					{
						vcSystem::Error("operator module " + parent_module->Get_Label() + " reads from pipe " + p->Get_Id());
						continue;
					}

					ofile << comma << endl;
					ofile << p->Get_VHDL_Pipe_Interface_Port_Name("read_req") 
						<< " => " 
						<< p->Get_Pipe_Aggregate_Section("read_req", 
								hindex, 
								lindex) 
						<< "," << endl;
					ofile << p->Get_VHDL_Pipe_Interface_Port_Name("read_ack") 
						<< " => " 
						<< p->Get_Pipe_Aggregate_Section( "read_ack", 
								hindex, 
								lindex) 
						<< "," << endl;
					ofile << p->Get_VHDL_Pipe_Interface_Port_Name("read_data") 
						<< " => " 
						<< p->Get_Pipe_Aggregate_Section("read_data", 
								hindex, 
								lindex);
					comma = ",";
				}
			}
		}
	}

	pipe_set.clear();
	for(int idx = 0; idx < _compatible_outport_groups.size(); idx++)
	{
		vcPipe* p = ((vcOutport*) (*(_compatible_outport_groups[idx].begin())))->Get_Pipe();
		pipe_set.insert(p);
	}

	for(set<vcPipe*>::iterator iter = pipe_set.begin();
			iter != pipe_set.end();
			iter++)
	{
		vcPipe* p = *iter;

		if(p->Get_Parent() == NULL)
		{
			int hindex, lindex;
			if(p->Get_Pipe_Module_Section(parent_module,"write",hindex,lindex))
			{
				if(parent_module->Get_Volatile_Flag())
				{
					vcSystem::Error("volatile module " + parent_module->Get_Label() + " writes to pipe " + p->Get_Id());
					continue;
				}
				else if (parent_module->Get_Volatile_Flag())
				{
					vcSystem::Error("operator module " + parent_module->Get_Label() + " writes to pipe " + p->Get_Id());
					continue;
				}
				ofile << comma << endl;
				ofile << p->Get_VHDL_Pipe_Interface_Port_Name("write_req") 
					<< " => " 
					<< p->Get_Pipe_Aggregate_Section( "write_req", 
							hindex, 
							lindex) 
					<< "," << endl;
				ofile << p->Get_VHDL_Pipe_Interface_Port_Name("write_ack") 
					<< " => " 
					<< p->Get_Pipe_Aggregate_Section("write_ack", 
							hindex, 
							lindex) 
					<< "," << endl;
				ofile << p->Get_VHDL_Pipe_Interface_Port_Name("write_data") 
					<< " => " 
					<< p->Get_Pipe_Aggregate_Section("write_data", 
							hindex, 
							lindex);

				comma = ",";
			}
		}
	}

	return(comma);
}

string vcDataPath::Print_VHDL_Call_Interface_Port_Map(string comma, ostream& ofile)
{
	set<vcModule*,vcRoot_Compare> called_module_set;
	vcModule* called_module;
	vcModule* parent_module = this->Get_Parent();

	for(int idx = 0; idx < _compatible_call_groups.size(); idx++)
	{
		called_module = ((vcCall*) (*(_compatible_call_groups[idx].begin())))->Get_Called_Module();
		if(!(called_module->Get_Operator_Flag() || called_module->Get_Volatile_Flag()))
			called_module_set.insert(called_module);
	}

	for(set<vcModule*,vcRoot_Compare>::iterator iter = called_module_set.begin();
			iter != called_module_set.end();
			iter++)
	{
		called_module = (*iter);

		int hindex, lindex;
		if(called_module->Get_Caller_Module_Section(parent_module,hindex,lindex))
		{

			if(parent_module->Get_Volatile_Flag() && !called_module->Get_Volatile_Flag())
			{
				vcSystem::Error("volatile module " + parent_module->Get_Label() + " calls non-volatile module  " + called_module->Get_Label());
				continue;
			}
			else if (parent_module->Get_Volatile_Flag() && !(called_module->Get_Volatile_Flag() || called_module->Get_Operator_Flag()))
			{
				vcSystem::Error("operator module " + parent_module->Get_Label() + " calls non-operator/non-volatile module " + called_module->Get_Label());
				continue;
			}

			ofile << comma << endl;
			ofile << called_module->Get_VHDL_Call_Interface_Port_Name("call_reqs") << " => " <<
				called_module->Get_Aggregate_Section("call_reqs", hindex, lindex) << "," << endl;
			ofile << called_module->Get_VHDL_Call_Interface_Port_Name("call_acks") << " => " <<
				called_module->Get_Aggregate_Section("call_acks", hindex, lindex) << "," << endl;

			if(called_module->Get_In_Arg_Width() > 0)
			{
				ofile << called_module->Get_VHDL_Call_Interface_Port_Name("call_data") << " => " << 
					called_module->Get_Aggregate_Section("call_data", hindex, lindex) << "," << endl;
			}

			ofile << called_module->Get_VHDL_Call_Interface_Port_Name("call_tag") << " => " << 
				called_module->Get_Aggregate_Section("call_tag", hindex, lindex) << "," << endl;
			ofile << called_module->Get_VHDL_Call_Interface_Port_Name("return_reqs") << " => " << 
				called_module->Get_Aggregate_Section("return_reqs", hindex, lindex)<< "," << endl;
			ofile << called_module->Get_VHDL_Call_Interface_Port_Name("return_acks") << " => " << 
				called_module->Get_Aggregate_Section("return_acks", hindex, lindex)<< "," << endl;

			if(called_module->Get_Out_Arg_Width() > 0)
			{
				ofile << called_module->Get_VHDL_Call_Interface_Port_Name("return_data") << " => " << 
					called_module->Get_Aggregate_Section("return_data", hindex, lindex)<< "," << endl;
			}

			ofile << called_module->Get_VHDL_Call_Interface_Port_Name("return_tag") << " => " << 
				called_module->Get_Aggregate_Section("return_tag", hindex, lindex);
			comma = ",";
		}
	}
	return(comma);
}

/////////////////////////////////////////  miscellaneous utility functions ///////////////////////////////////////////

void Generate_Guard_Constants(string& buffering_const, string& guard_flag_const,
		vector<vcDatapathElement*>& ops, vector<vcWire*>& guard_wires)
{
	assert(guard_wires.size() == ops.size());
	int num_reqs = guard_wires.size();
	int idx;

	// guard flags.
	guard_flag_const = "constant guardFlags : BooleanArray(" + IntToStr(num_reqs-1) + " downto 0) := (";
	buffering_const  = "constant guardBuffering: IntegerArray(" + IntToStr(num_reqs-1) + " downto 0)  := (";
	for(idx = num_reqs-1; idx >= 0; idx--)
	{
		int lidx = (num_reqs - 1) - idx;
		if(guard_wires[idx] != NULL)
		{
			guard_flag_const += IntToStr(lidx) + " => true";
		}
		else
		{
			guard_flag_const += IntToStr(lidx) + " => false";
		}

		vcDatapathElement* dpe = ops[idx];

		int B = 0;
		if(dpe->Is_Part_Of_Pipeline())
		{
			B = dpe->Get_Delay();
		}
		if (B < 2) 
			B = 2; // to cut combi paths.

		buffering_const +=  IntToStr(lidx)  + " => " + IntToStr(B);
		if(idx != 0)
		{
			guard_flag_const  += ", ";
			buffering_const   += ", ";
		}
	}
	guard_flag_const += ");";
	buffering_const += ");";
}

void Print_VHDL_Concatenate_Req(string req_id, vector<vcTransition*>& reqs,  ostream& ofile)
{

	for(int rI = 0; rI < reqs.size(); rI++)
	{
		ofile << req_id <<  "(" << (reqs.size()-1)-rI << ") <= " 
			<< reqs[rI]->Get_CP_To_DP_Symbol() << ";" << endl;
	}
}


void Print_VHDL_Disconcatenate_Ack(string ack_id, vector<vcTransition*>& acks,  ostream& ofile)
{
	// disconcatenate ackL
	for(int aI = 0; aI < acks.size(); aI++)
	{
		if(acks[aI] != NULL)
			ofile << acks[aI]->Get_DP_To_CP_Symbol() << " <= "
				<< ack_id << "(" << (acks.size()-1) - aI << ");" << endl;
	}
}



void Print_VHDL_Concatenation(string target, vector<vcWire*> wires, ostream& ofile)
{
	ofile << target << " <= ";
	for(int u = 0; u < wires.size(); u++)
	{
		if(u > 0)
			ofile << " & ";
		ofile << wires[u]->Get_VHDL_Signal_Id();
	}
	ofile << ";" << endl;
}



void Print_VHDL_Disconcatenation(string source, int total_width, vector<vcWire*> wires, ostream& ofile)
{
	int lindex = total_width-1;
	for(int u = 0; u < wires.size(); u++)
	{
		ofile << wires[u]->Get_VHDL_Signal_Id() << " <= " << source << "(";
		ofile << lindex << " downto " << (lindex - (wires[u]->Get_Size()-1)) << ");" << endl;
		lindex -= wires[u]->Get_Size();
	}
}

void Print_VHDL_Guard_Concatenation(int num_reqs, string guard_vector, vector<vcWire*>& guard_wires, vector<bool>& guard_complements, ostream& ofile)
{
	int idx;
	for(idx = num_reqs-1; idx >= 0; idx--)
	{
		int lidx = (num_reqs - 1) - idx;
		if(guard_wires[idx] != NULL)
		{
			if(guard_complements[idx])
				ofile << guard_vector << "(" << lidx << ")  <=  not " << guard_wires[idx]->Get_VHDL_Signal_Id() << "(0);" << endl;
			else
				ofile << guard_vector << "(" << lidx << ")  <= " << guard_wires[idx]->Get_VHDL_Signal_Id() << "(0);" << endl;
		}
		else
			ofile << guard_vector << "(" << lidx << ")  <=  '1';" << endl;

	}
}

void Print_VHDL_Guard_Instance_Level(bool sample_only, bool update_only,
		string inst_id, int num_reqs, string buffering, string guard_flags, string guards, 
		string sr_in, string sa_out,  string sr_out, string sa_in,
		string cr_in, string ca_out,  string cr_out, string ca_in, ostream& ofile)
{
	ofile << inst_id << ": SplitGuardInterface generic map(name => \"" << inst_id << "\", nreqs => " << num_reqs
		<< ", buffering => " << buffering << ", use_guards => " << guard_flags << ", "
		<< " sample_only => " << (sample_only ? "true" : "false" ) << ", " 
		<< " update_only => " << (update_only ? "true" : "false" ) 
		<< ") -- {" << endl
		<< "port map(clk => clk, reset => reset," << endl
		<< "sr_in => " << sr_in << "," << endl
		<< "sr_out => " << sr_out << "," << endl
		<< "sa_in => " << sa_in << "," << endl
		<< "sa_out => " << sa_out << "," << endl
		<< "cr_in => " << cr_in << "," << endl
		<< "cr_out => " << cr_out << "," << endl
		<< "ca_in => " << ca_in << "," << endl
		<< "ca_out => " << ca_out << "," << endl
		<< "guards => " << guards << "); -- }" << endl;
}

   
//
// This is a new effort:  a data-pipeline is a connected component of 
// The following graph.
//
// Vertices of the graph are data-path elements.
//
// There is a path from vertex i -> vertex j if 
//   1. j is the unique successor of i.
//   2. There is a producer consumer dependency from i -> j.
//
// Each connected component can be transformed to a pipeline
// using the following elements
//   a. Pulse to level converters at the inputs to the sources
//   b. Replace pulse joins by level joins.
//   c. Operators to be replaced by level ops..
// etc. TBD. 
//
void vcDataPath::Build_Data_Pipelines ()
{
  assert (0);
	/*
  vector<vcDatapathElement*> sinks;

  // first find all the sinks for the candidate pipelines.
  for(map<string, vcDatapathElement*>::iterator dpe_iter = _dpe_map.begin();
      dpe_iter != _dpe_map.end();
      dpe_iter++)
    {
	vcDatapathElement* dpe = (*dpe_iter).second;
	if(!dpe->Get_Flow_Through() && (dpe->_output_wires.size() == 1))
	{
		vcWire* w = dpe->_output_wires[0];
		if(w->Get_Number_Of_Receivers() == 1)
		{
      			if((dpe->Kind() == "vcUnarySplitOperator") ||
	 			(dpe->Kind() == "vcBinarySplitOperator") ||
					(dpe->Kind == "vcSelect"))
			{
				sinks.push_back(dpe);
			}
		}
	}
    }

     // starting from the sinks, look "back" to identify the maximal fan-in regions
     //  1. which consist of only unary/binary/ternary operators.
     //  2. which 
	*/
	
}


