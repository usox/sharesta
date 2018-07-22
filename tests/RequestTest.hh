<?hh // strict
namespace Usox\Sharesta;

use function Facebook\FBExpect\expect;

class RequestTest extends \Facebook\HackTest\HackTestCase {

	public function testRequestEstablishesPropertiesOnCreation(): void {
		$request = new Request(
			dict([
				'REQUEST_URI' => '/api/test/sub/1',
				'REQUEST_METHOD' => 'GET'
			]),
			dict([
				'a' => '1',
				'b' => '2'
			]),
			new RequestBody()
		);
		$uri_values = $request->getUriValues();

		expect($request->getRoute('/api/'))->toBeSame('test/sub/1');
		expect($request->getHttpMethod())->toBeSame('GET');
		expect($uri_values['b'])->toBeSame('2');
	}
}
