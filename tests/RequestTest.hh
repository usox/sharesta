<?hh
namespace Usox\Sharesta;

class RequestTest extends \PHPUnit_Framework_TestCase {

	public function testRequestEstablishesPropertiesOnCreation() {
		$_SERVER['REQUEST_URI'] = '/api/test/sub/1';
		$_SERVER['REQUEST_METHOD'] = 'GET';
		$_GET = array('a' => '1', 'b' => '2');

		$request = new Request(
			'/api/',
			new Map($_SERVER),
			new Map($_GET),
			new RequestBody()
		);
		$uri_values = $request->getUriValues();

		$this->assertEquals('test/sub/1', $request->getRoute());
		$this->assertEquals('GET', $request->getHttpMethod());
		$this->assertEquals('2', $uri_values->get('b'));
	}
}
