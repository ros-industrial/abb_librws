#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include <abb_librws/rws_rapid.h>

using namespace pybind11::literals;
namespace py = pybind11;

namespace abb {
namespace rws {

void init_rapid(py::module &m){

    py::class_<RAPIDBool>(m, "RAPIDBool")
        .def(py::init<const bool>(), "bool"_a = false)
        .def_readwrite("value", &RAPIDBool::value)
    ;
    py::implicitly_convertible<bool, RAPIDBool>();

    py::class_<RAPIDNum>(m, "RAPIDNum")
        .def(py::init<const double>(), "float"_a = 0.0)
        .def_readwrite("value", &RAPIDNum::value)
    ;
    py::implicitly_convertible<float, RAPIDNum>();

    py::class_<RAPIDDnum>(m, "RAPIDDnum")
        .def(py::init<const double>(), "float"_a = 0.0)
        .def_readwrite("value", &RAPIDDnum::value)
    ;
    py::implicitly_convertible<float, RAPIDDnum>();

    py::class_<RAPIDString>(m, "RAPIDString")
        .def(py::init<const std::string&>(), "string"_a = "")
        .def_readwrite("value", &RAPIDString::value)
    ;
    py::implicitly_convertible<std::string, RAPIDString>();

    py::class_<RAPIDSymbolDataAbstract>(m, "RAPIDSymbolDataAbstract");
    py::class_<RAPIDRecord, RAPIDSymbolDataAbstract>(m, "RAPIDRecord")
        .def(py::init<const std::string&>(), "record_type_name"_a)
        .def("__repr__", &RAPIDRecord::constructString)
    ;

    py::class_<RobJoint, RAPIDRecord>(m, "RobJoint")
        .def(py::init<>())
        .def(py::init(
            [](RAPIDNum& rax_1, RAPIDNum& rax_2, RAPIDNum& rax_3, RAPIDNum& rax_4, RAPIDNum& rax_5, RAPIDNum& rax_6) {
                RobJoint* r = new RobJoint();
                r->rax_1 = rax_1;
                r->rax_2 = rax_2;
                r->rax_3 = rax_3;
                r->rax_4 = rax_4;
                r->rax_5 = rax_5;
                r->rax_6 = rax_6;
                return r;
            }),
            "rax_1"_a = 0.0, "rax_2"_a = 0.0,
            "rax_3"_a = 0.0, "rax_4"_a = 0.0,
            "rax_5"_a = 0.0, "rax_6"_a = 0.0
        )
        .def(py::init(
            [](py::array_t<double> arr) {
                auto arr_u = arr.unchecked<1>();
                if (arr_u.size() != 6) {
                    throw std::length_error("Input array is not of length 6!");
                }
                RobJoint* r = new RobJoint();
                r->rax_1 = arr_u(0);
                r->rax_2 = arr_u(1);
                r->rax_3 = arr_u(2);
                r->rax_4 = arr_u(3);
                r->rax_5 = arr_u(4);
                r->rax_6 = arr_u(5);
                return r;
            }), 
            "rax"_a
        )
        .def_readwrite("rax_1", &RobJoint::rax_1)
        .def_readwrite("rax_2", &RobJoint::rax_2)
        .def_readwrite("rax_3", &RobJoint::rax_3)
        .def_readwrite("rax_4", &RobJoint::rax_4)
        .def_readwrite("rax_5", &RobJoint::rax_5)
        .def_readwrite("rax_6", &RobJoint::rax_6)
 ;

    py::class_<ExtJoint, RAPIDRecord>(m, "ExtJoint")
        .def(py::init<>())
        .def(py::init(
            [](RAPIDNum& eax_a, RAPIDNum& eax_b, RAPIDNum& eax_c, RAPIDNum& eax_d, RAPIDNum& eax_e, RAPIDNum& eax_f) {
                ExtJoint* e = new ExtJoint();
                e->eax_a = eax_a;
                e->eax_b = eax_b;
                e->eax_c = eax_c;
                e->eax_d = eax_d;
                e->eax_e = eax_e;
                e->eax_f = eax_f;
                return e;
            }),
            "eax_a"_a = 9e9, "eax_b"_a = 9e9,
            "eax_c"_a = 9e9, "eax_d"_a = 9e9,
            "eax_e"_a = 9e9, "eax_f"_a = 9e9
        )
        .def(py::init(
            [](py::array_t<double> arr) {
                auto arr_u = arr.unchecked<1>();
                if (arr_u.size() != 6) {
                    throw std::length_error("Input array is not of length 6!");
                }
                ExtJoint* e = new ExtJoint();
                e->eax_a = arr_u(0);
                e->eax_b = arr_u(1);
                e->eax_c = arr_u(2);
                e->eax_d = arr_u(3);
                e->eax_e = arr_u(4);
                e->eax_f = arr_u(5);
                return e;
            }),
            "eax"_a
        )
        .def_readwrite("eax_a", &ExtJoint::eax_a)
        .def_readwrite("eax_b", &ExtJoint::eax_b)
        .def_readwrite("eax_c", &ExtJoint::eax_c)
        .def_readwrite("eax_d", &ExtJoint::eax_d)
        .def_readwrite("eax_e", &ExtJoint::eax_e)
        .def_readwrite("eax_f", &ExtJoint::eax_f)
    ;

    py::class_<Pos, RAPIDRecord>(m, "Pos")
        .def(py::init<>())
        .def(py::init(
            [](RAPIDNum& x, RAPIDNum& y, RAPIDNum& z) {
                Pos* p = new Pos();
                p->x = x;
                p->y = y;
                p->z = z;
                return p;
            }),
            "x"_a = 0.0, "y"_a = 0.0, "z"_a = 0.0
        )
        .def(py::init(
            [](py::array_t<double> arr) {
                auto arr_u = arr.unchecked<1>();
                if (arr_u.size() != 3) {
                    throw std::length_error("Input array is not of length 3!");
                }
                Pos* p = new Pos();
                p->x = arr_u(0);
                p->y = arr_u(1);
                p->z = arr_u(2);
                return p;
            }),
            "pos"_a
        )
        .def_readwrite("x", &Pos::x)
        .def_readwrite("y", &Pos::y)
        .def_readwrite("z", &Pos::z)
    ;

    py::class_<Orient, RAPIDRecord>(m, "Orient")
        .def(py::init<>())
        .def(py::init(
            [](RAPIDNum& q1, RAPIDNum& q2, RAPIDNum& q3, RAPIDNum& q4) {
                Orient* o = new Orient();
                o->q1 = q1;
                o->q2 = q2;
                o->q3 = q3;
                o->q4 = q4;
                return o;
            })
        )
        .def(py::init(
            [](py::array_t<double> arr) {
                auto arr_u = arr.unchecked<1>();
                if (arr_u.size() != 4) {
                    throw std::length_error("Input array is not of length 4!");
                }
                Orient* o = new Orient();
                o->q1 = arr_u(0);
                o->q2 = arr_u(1);
                o->q3 = arr_u(2);
                o->q4 = arr_u(3);
                return o;
            })
        )
        // TODO: allow for orientation constructor that uses rotation matrix
        .def_readwrite("q1", &Orient::q1)
        .def_readwrite("q2", &Orient::q2)
        .def_readwrite("q3", &Orient::q3)
        .def_readwrite("q4", &Orient::q4)
    ;

    py::class_<Pose, RAPIDRecord>(m, "Pose")
        .def(py::init<>())
        .def(py::init<const Pose&>())
        .def(py::init(
            [](const Pos& pos, const Orient& rot) {
                Pose* pose = new Pose();
                pose->pos = pos;
                pose->rot = rot;
                return pose;
            })
        )
        // TODO: make 4x4 pose array into a pose object
        // .def(py::init(
        //     [](py::array_t<double> arr) {
        //         auto arr_u = arr.unchecked<2>();
        //         if (arr_u.shape(0) != 4 || arr_u.shape(1) != 4) {
        //             throw std::length_error("Input matrix is not of size (4, 4)!");
        //         }
        //         Pose* pose = new Pose();
        //         pose->pos = pos;
        //         pose->rot = rot;
        //         return pose;
        //     })
        // )
        .def_readwrite("pos", &Pose::pos)
        .def_readwrite("rot", &Pose::rot)
    ;

    py::class_<ConfData, RAPIDRecord>(m, "ConfData")
        .def(py::init<>())
        .def(py::init(
            [](RAPIDNum& cf1, RAPIDNum& cf4, RAPIDNum& cf6, RAPIDNum& cfx) {
                ConfData* cd = new ConfData();
                cd->cf1 = cf1;
                cd->cf4 = cf4;
                cd->cf6 = cf6;
                cd->cfx = cfx;
                return cd;
            }),
            "cf1"_a, "cf4"_a, "cf6"_a, "cfx"_a
        )
        .def(py::init(
            [](py::array_t<double> arr) {
                auto arr_u = arr.unchecked<1>();
                if (arr_u.size() != 4) {
                    throw std::length_error("Input array is not of length 4!");
                }
                ConfData* cd = new ConfData();
                cd->cf1 = arr_u(0);
                cd->cf4 = arr_u(1);
                cd->cf6 = arr_u(2);
                cd->cfx = arr_u(3);
                return cd;
            }),
            "conf_data"_a
        )
        .def_readwrite("cf1", &ConfData::cf1)
        .def_readwrite("cf4", &ConfData::cf4)
        .def_readwrite("cf6", &ConfData::cf6)
        .def_readwrite("cfx", &ConfData::cfx)
    ;

    py::class_<RobTarget, RAPIDRecord>(m, "RobTarget")
        .def(py::init<>())
        .def(py::init<const RobTarget&>())
        .def(py::init(
            [](const Pos& pos, const Orient& orient, const ConfData& robconf, const ExtJoint& extax) {
                RobTarget* rt = new RobTarget();
                rt->pos = pos;
                rt->orient = orient;
                rt->robconf = robconf;
                rt->extax = extax;
                return rt;
            }),
            "pos"_a, "orient"_a, "robconf"_a, "extax"_a
        )
        .def_readwrite("pos", &RobTarget::pos)
        .def_readwrite("orient", &RobTarget::orient)
        .def_readwrite("robconf", &RobTarget::robconf)
        .def_readwrite("extax", &RobTarget::extax)
    ;

    py::class_<JointTarget, RAPIDRecord>(m, "JointTarget")
        .def(py::init<>())
        .def(py::init<const JointTarget&>())
        .def(py::init(
            [](const RobJoint& robax, const ExtJoint& extax) {
                JointTarget* jt = new JointTarget();
                jt->robax = robax;
                jt->extax = extax;
                return jt;
            }),
            "robax"_a, "extax"_a
        )
        .def_readwrite("robax", &JointTarget::robax)
        .def_readwrite("extax", &JointTarget::extax)
    ;

    py::class_<SpeedData, RAPIDRecord>(m, "SpeedData")
        .def(py::init<>())
        .def(py::init(
            [](RAPIDNum& v_tcp, RAPIDNum& v_ori, RAPIDNum& v_leax, RAPIDNum& v_reax) {
                SpeedData* sd = new SpeedData();
                sd->v_tcp = v_tcp;
                sd->v_ori = v_ori;
                sd->v_leax = v_leax;
                sd->v_reax = v_reax;
                return sd;
            }),
            "v_tcp"_a, "v_ori"_a, "v_leax"_a, "v_reax"_a
        )
        .def(py::init(
            [](py::array_t<double, py::array::forcecast> arr) {
                auto arr_u = arr.unchecked<1>();
                if (arr_u.size() != 4) {
                    throw std::length_error("Input array is not of length 4!");
                }
                SpeedData* sd = new SpeedData();
                sd->v_tcp = arr_u(0);
                sd->v_ori = arr_u(1);
                sd->v_leax = arr_u(2);
                sd->v_reax = arr_u(3);
                return sd;
            }),
            "speed_data"_a
        )
        .def_readwrite("v_tcp", &SpeedData::v_tcp)
        .def_readwrite("v_ori", &SpeedData::v_ori)
        .def_readwrite("v_leax", &SpeedData::v_leax)
        .def_readwrite("v_reax", &SpeedData::v_reax)
    ;

    py::class_<ZoneData, RAPIDRecord>(m, "ZoneData")
        .def(py::init<>())
        .def(py::init(
            [](RAPIDBool& finep, RAPIDNum& pzone_tcp, RAPIDNum& pzone_ori, RAPIDNum& pzone_eax, RAPIDNum& zone_ori, RAPIDNum& zone_leax, RAPIDNum& zone_reax) {
                ZoneData* zd = new ZoneData();
                zd->finep = finep;
                zd->pzone_tcp = pzone_tcp;
                zd->pzone_ori = pzone_ori;
                zd->pzone_eax = pzone_eax;
                zd->zone_ori = zone_ori;
                zd->zone_leax = zone_leax;
                zd->zone_reax = zone_reax;
                return zd;
            }),
            "finep"_a, "pzone_tcp"_a, "pzone_ori"_a, "pzone_eax"_a, "zone_ori"_a, "zone_leax"_a, "zone_reax"_a
        )
        .def(py::init(
            [](py::array_t<double, py::array::forcecast> arr) {
                auto arr_u = arr.unchecked<1>();
                if (arr_u.size() != 7) {
                    throw std::length_error("Input array is not of length 7!");
                }
                ZoneData* zd = new ZoneData();
                zd->finep = arr_u(0);
                zd->pzone_tcp = arr_u(1);
                zd->pzone_ori = arr_u(2);
                zd->pzone_eax = arr_u(3);
                zd->zone_ori = arr_u(4);
                zd->zone_leax = arr_u(5);
                zd->zone_reax = arr_u(6);
                return zd;
            }),
            "zone_data"_a
        )
        .def_readwrite("finep", &ZoneData::finep)
        .def_readwrite("pzone_tcp", &ZoneData::pzone_tcp)
        .def_readwrite("pzone_ori", &ZoneData::pzone_ori)
        .def_readwrite("pzone_eax", &ZoneData::pzone_eax)
        .def_readwrite("zone_ori", &ZoneData::zone_ori)
        .def_readwrite("zone_leax", &ZoneData::zone_leax)
        .def_readwrite("zone_reax", &ZoneData::zone_reax)
    ;

}
} // end namespace rws
} // end namespace abb
