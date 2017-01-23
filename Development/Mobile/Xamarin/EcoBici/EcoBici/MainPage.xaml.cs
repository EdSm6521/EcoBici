
/********************************************************/

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using System.Net.Http;
using System.Text;
using System.Windows;
using System.Threading;
using System.Threading.Tasks;



// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=391641

namespace EcoBici
{

    public sealed partial class MainPage : Page
    {

        static HttpClient gHttpClient = new HttpClient();

        public MainPage()
        {
            this.InitializeComponent();

            // this.NavigationCacheMode = NavigationCacheMode.Required;

            gHttpClient.BaseAddress = new Uri("http://192.168.4.1");



        }

        static string asyncResult = "";

        static async Task DownloadPageAsync(string _sa)
        {
            asyncResult = "";
            string page = "http://192.168.4.1" + _sa;
            HttpClient client = new HttpClient();
            using (HttpResponseMessage response = await client.PostAsync(page, null))
            using (HttpContent content = response.Content)
            {
                string result = await content.ReadAsStringAsync();
                asyncResult = result;
            }
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            // TODO: Prepare page for display here.

            // TODO: If your application contains multiple pages, ensure that you are
            // handling the hardware Back button by registering for the
            // Windows.Phone.UI.Input.HardwareButtons.BackPressed event.
            // If you are using the NavigationHelper provided by some templates,
            // this event is handled for you.
        }

        private async void getData()
        {
            await DownloadPageAsync("/get_gps_data");
            textBlock.Text = "[S]" + asyncResult + "[E]";
        }

        private void textBlock_Tapped(object sender, TappedRoutedEventArgs e)
        {
            getData();
            gHttpClient.CancelPendingRequests();
        }

        private void textBlock_SelectionChanged(object sender, RoutedEventArgs e)
        {

        }
    }
}
