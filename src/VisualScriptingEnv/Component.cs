using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using GUICLR;

namespace VisualScripting
{
    public class Component : IDisposable
    {
        GUICLR.Component _component;
        GUICLR.BoundingBox _boundingBox;
        public Component()
        {
            _component = new GUICLR.Component("Component");
            _boundingBox = _component.getBoundingBox();
        }

        //BoundingBox

        public void addSlider(string title, SliderState sliderState, float currentValue, float startValue, float endValue)
        {
            _component.addSlider(title, sliderState, currentValue, startValue, endValue);
        }

        private Color _color;
        public Component(string title)
        {
            _component = new GUICLR.Component(title);
            _color = Color.FromArgb(255, 103, 0);
            int r, g, b;
            r = ((int)_color.R);
            g = ((int)_color.G);
            b = ((int)_color.B);
            _component.setColor(r, g, b);
        }

        public Component(string title, Color color)
        {
            _component = new GUICLR.Component(title);
            _color = color;
            int r, g, b;
            r = ((int)_color.R);
            g = ((int)_color.G);
            b = ((int)_color.B);
            _component.setColor(r, g, b);
        }

        public string Name
        {
            get
            {
                return _component.getTitle();
            }
            set
            {
                _component.setTitle(value);
            }
        }

        public Color Color
        {
            get
            {
                return _color;
            }
            set
            {
                _color = value;
                int r, g, b;
                r = ((int)_color.R);
                g = ((int)_color.G);
                b = ((int)_color.B);
                _component.setColor(r, g, b);
            }
        }

        //BoundingBox

        public void addInputSlider(string title, float currentValue, float startValue, float endValue)
        {
            _component.addSlider(title, SliderState.INPUT, currentValue, startValue, endValue);
        }

        public ComponentCollection Children { get; }

        public void Dispose()
        {
            _component.Dispose();
        }

        ~Component()
        {
            this.Dispose();
        }
    }

    public class ComponentCollection : IList<Component>, ICollection<Component>, IEnumerable<Component>
    {
        private Component[] _components;
        private Component _parent;

        //BoundingBox

        public Component this[int index] { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

        public int Count => _components.Length;

        public bool IsReadOnly => false;

        public void Add(Component item)
        {
            throw new NotImplementedException();
        }

        public void Clear()
        {
            _components = new Component[] { };
        }

        public bool Contains(Component item)
        {
            throw new NotImplementedException();
        }

        public void CopyTo(Component[] array, int arrayIndex)
        {
            throw new NotImplementedException();
        }

        public IEnumerator<Component> GetEnumerator()
        {
            return (IEnumerator<Component>)_components.GetEnumerator();
        }

        public int IndexOf(Component item)
        {
            throw new NotImplementedException();
        }

        public void Insert(int index, Component item)
        {
            throw new NotImplementedException();
        }

        public bool Remove(Component item)
        {
            throw new NotImplementedException();
        }

        public void RemoveAt(int index)
        {
            throw new NotImplementedException();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return _components.GetEnumerator();
        }
    }
}
