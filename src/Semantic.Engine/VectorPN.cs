using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    public struct VectorPN
    {
        private const double eps = 1E-07;
        private int dim;
        private double[] data;

        public int Label;

        public double this[int i]
        {
            get
            {
                if (i < this.Dim)
                {
                    return data[i];
                }
                else
                {
                    return 0;
                }
            }
            set
            {
                if (i < this.Dim)
                {
                    data[i] = value;
                }
            }
        }

        public IEnumerable<double> Data { get { return data; } }

        public int Dim { get { return this.dim; } }

        public VectorPN(int dim)
        {
            this.Label = -1;
            this.dim = dim;
            this.data = new double[dim];
        }

        public VectorPN(VectorPN vec)
        {
            this.dim = vec.dim;
            this.Label = vec.Label;
            this.data = new double[vec.dim];
            for (int i = 0; i < vec.dim; i++)
            {
                this.data[i] = vec[i];
            }
        }

        private static bool Equals(VectorPN vec0, VectorPN vec1)
        {
            if (vec0.Dim == vec1.Dim)
            {
                bool result = true;
                for (int i = 0; i < vec0.Dim; i++)
                {
                    result = result && VectorPN.EqualsWithinEpsilon(vec0[i], vec1[i]);
                }
                return result;
            }
            return false;
        }

        public static bool operator ==(VectorPN vec0, VectorPN vec1)
        {
            return (vec0 == null && vec1 == null) ||
                (vec0 != null && vec1 != null && Equals(vec0, vec1));
        }

        public static bool operator !=(VectorPN vec0, VectorPN vec1)
        {
            return !(vec0 == vec1);
        }

        public static VectorPN operator +(VectorPN vec0, VectorPN vec1)
        {
            return vec0.Add(vec1);
        }

        public static VectorPN operator -(VectorPN vec0, VectorPN vec1)
        {
            return vec0.Subtract(vec1);
        }

        public static VectorPN operator -(VectorPN vec)
        {
            return vec.Negate();
        }

        public static double operator *(VectorPN vec0, VectorPN vec1)
        {
            return vec0.Dot(vec1);
        }

        private static bool EqualsWithinEpsilon(double d0, double d1)
        {
            return Math.Abs(d0 - d1) < eps;
        }

        public void AddTo(VectorPN right)
        {
            if (this.Dim == right.Dim)
            {
                for (int i = 0; i < this.Dim; i++)
                {
                    this.data[i] += right[i];
                }
            }
        }

        public bool CheckDimension(VectorPN vec)
        {
            return this.Dim == vec.Dim;
        }

        public VectorPN Add(VectorPN vec)
        {
            VectorPN result = new VectorPN(this.Dim);
            for (int i = 0; i < this.Dim; i++)
            {
                result[i] = this.data[i] + vec[i];
            }
            return result;
        }

        public VectorPN Subtract(VectorPN vec)
        {
            VectorPN result = new VectorPN(this.Dim);
            for (int i = 0; i < this.Dim; i++)
            {
                result[i] = this.data[i] - vec[i];
            }
            return result;
        }

        public VectorPN Negate()
        {
            VectorPN result = new VectorPN(this.Dim);
            for (int i = 0; i < this.Dim; i++)
            {
                result[i] = -this.data[i];
            }
            return result;
        }

        public double Dot(VectorPN vec)
        {
            if (!CheckDimension(vec))
                return 0;
            double result = 0;
            for (int i = 0; i < this.Dim; i++)
            {
                result += this.data[i] * vec[i];
            }
            return result;
        }

        public double Length()
        {
            return Math.Sqrt(this * this);
        }

        public bool Equals(VectorPN other)
        {
            return this == other;
        }

        public override bool Equals(object o)
        {
            if (this.GetType() == o.GetType())
                return this == (VectorPN)o;
            else
                return false;
        }

        public override int GetHashCode()
        {
            if (this.Dim >= 1)
            {
                int result = this.data[0].GetHashCode();
                for (int i = 1; i < this.Dim; i++)
                {
                    result ^= this.data[i].GetHashCode();
                }
                return result;
            }
            return 0;
        }

        public override string ToString()
        {
            return string.Join(",", this.data);
        }

    }
}
